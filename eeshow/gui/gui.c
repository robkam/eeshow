/*
 * gui/gui.c - GUI for eeshow
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Resources:
 *
 * http://zetcode.com/gfx/cairo/cairobackends/
 * https://developer.gnome.org/gtk3/stable/gtk-migrating-2-to-3.html
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "version.h"
#include "misc/util.h"
#include "misc/diag.h"
#include "file/git-hist.h"
#include "kicad/ext.h"
#include "kicad/pl.h"
#include "kicad/lib.h"
#include "kicad/sch.h"
#include "kicad/pro.h"
#include "kicad/delta.h"
#include "gui/aoi.h"
#include "gui/input.h"
#include "gui/common.h"
#include "gui/icons.h"
#include "gui/gui.h"


/* ----- Helper functions -------------------------------------------------- */


struct gui_sheet *sheets(const struct gui *gui)
{
	if (gui->old_hist && gui->diff_mode == diff_old)
		return gui->old_hist->sheets;
	else
		return gui->new_hist->sheets;
}


struct gui_sheet *find_corresponding_sheet(struct gui_sheet *pick_from,
    struct gui_sheet *ref_in, const struct gui_sheet *ref)
{
	struct gui_sheet *sheet, *plan_b;
	const char *title = ref->sch->title;

	/* plan A: try to find sheet with same name */

	if (title)
		for (sheet = pick_from; sheet; sheet = sheet->next)
			if (sheet->sch->title &&
			    !strcmp(title, sheet->sch->title))
				return sheet;

	/* plan B: use sheet in same position in sheet sequence */

	plan_b = ref_in;
	for (sheet = pick_from; sheet; sheet = sheet->next) {
		if (plan_b == ref)
			return sheet;
		plan_b = plan_b->next;
	}

	/* plan C: just go to the top */
	return pick_from;
}


struct gui_sheet *current_sheet(const struct gui *gui)
{
	if (!gui->old_hist || gui->diff_mode != diff_old)
		return gui->curr_sheet;
	return find_corresponding_sheet(gui->old_hist->sheets,
	    gui->new_hist->sheets, gui->curr_sheet);
}


/* ----- AoIs -------------------------------------------------------------- */


struct sheet_aoi_ctx {
	struct gui *gui;
	const struct sch_obj *obj;
};


static void select_subsheet(void *user)
{
	const struct sheet_aoi_ctx *aoi_ctx = user;
	struct gui *gui = aoi_ctx->gui;
	const struct sch_obj *obj = aoi_ctx->obj;
	struct gui_sheet *sheet;

	if (!obj->u.sheet.sheet)
		return;

	if (!gui->old_hist || gui->diff_mode != diff_old) {
		for (sheet = gui->new_hist->sheets; sheet; sheet = sheet->next)
			if (sheet->sch == obj->u.sheet.sheet) {
				go_to_sheet(gui, sheet);
				return;
			}
		BUG("new sheet not found");
	}

	for (sheet = gui->old_hist->sheets; sheet; sheet = sheet->next)
		if (sheet->sch == obj->u.sheet.sheet)
			goto found;
	BUG("old sheet not found");

found:
	sheet = find_corresponding_sheet(gui->new_hist->sheets,
	    gui->old_hist->sheets, sheet);
	go_to_sheet(gui, sheet);
}


static void add_sheet_aoi(struct gui *gui, struct gui_sheet *parent,
    const struct sch_obj *obj)
{
	struct sheet_aoi_ctx *aoi_ctx = alloc_type(struct sheet_aoi_ctx);

	aoi_ctx->gui = gui;
	aoi_ctx->obj = obj;

	struct aoi aoi = {
		.x	= obj->x,
		.y	= obj->y,
		.w	= obj->u.sheet.w,
		.h	= obj->u.sheet.h,
		.click	= select_subsheet,
		.user	= aoi_ctx,
	};

	aoi_add(&parent->aois, &aoi);
}


/* ----- Load revisions ---------------------------------------------------- */


void mark_aois(struct gui *gui, struct gui_sheet *sheet)
{
	const struct sch_obj *obj;

	sheet->aois = NULL;
	for (obj = sheet->sch->objs; obj; obj = obj->next)
		switch (obj->type) {
		case sch_obj_sheet:
			add_sheet_aoi(gui, sheet, obj);
			break;
		case sch_obj_glabel:
			add_glabel_aoi(sheet, obj);
			break;
		case sch_obj_comp:
			add_comp_aoi(sheet, obj);
			break;
		default:
			break;
		}
}


static struct gui_sheet *get_sheets(struct gui *gui, struct gui_hist *hist,
    const struct sheet *sheets)
{
	const struct sheet *sch;
	struct gui_sheet *gui_sheets = NULL;
	struct gui_sheet **next = &gui_sheets;
	struct gui_sheet *new;

	for (sch = sheets; sch; sch = sch->next) {
		new = alloc_type(struct gui_sheet);
		new->sch = sch;
		new->gui = gui;
		new->hist = hist;
		new->gfx_thumb = NULL;
		new->thumb_surf = NULL;
		new->rendered = 0;

		new->over = NULL;
		new->aois = NULL;

		*next = new;
		next = &new->next;
	}
	*next = NULL;
	return gui_sheets;
}


/*
 * Library caching:
 *
 * We reuse previous components if all libraries are identical
 *
 * Future optimizations:
 * - don't parse into single list of components, so that we can share
 *   libraries that are the same, even if there are others that have changed.
 * - maybe put components into tree, so that they can be replaced individually
 *   (this would also help to identify sheets that don't need parsing)
 *
 * Sheet caching:
 *
 * We reuse previous sheets if
 * - all libraries are identical (whether a given sheet uses them or not),
 * - they have no sub-sheets, and
 * - the objects IDs (hashes) are identical.
 *
 * Note that we only compare with the immediately preceding (newer) revision,
 * so branches and merges can disrupt caching.
 *
 * Possible optimizations:
 * - if we record which child sheets a sheet has, we could also clone it,
 *   without having to parse it. However, this is somewhat complex and may
 *   not save all that much time.
 * - we could record what libraries a sheet uses, and parse only if one of
 *   these has changed (benefits scenarios with many library files),
 * - we could record what components a sheet uses, and parse only if one of
 *   these has changed (benefits scenarios with few big libraries),
 * - we could postpone library lookups to render time.
 * - we could record IDs globally, which would help to avoid tripping over
 *   branches and merges.
 */

static const struct sheet *parse_files(struct gui_hist *hist,
    const struct file_names *fn, bool recurse, struct gui_hist *prev)
{
	char *rev = NULL;
	struct file pro_file, sch_file;
	const struct file *leader = NULL;
	unsigned libs_open, i;
	bool libs_cached = 0;
	bool ok;

	if (hist->vcs_hist && hist->vcs_hist->commit)
		rev = vcs_git_get_rev(hist->vcs_hist);

	if (fn->pro) {
		if (file_open_revision(&pro_file, rev, fn->pro, NULL)) {
			free(rev);
			rev = NULL; /* thus sch_file opens as with file_open */
			fn = pro_parse_file(&pro_file, fn);
			if (!fn)
				return NULL;
			leader = &pro_file;
		} else {
			/*
			 * If we happen to have a top sheet name, we may as
			 * well try to use it.
			 */
			if (!fn->sch) {
				free(rev);
				return NULL;
			}
		}
	}
	sch_init(&hist->sch_ctx, recurse);
	ok = file_open_revision(&sch_file, rev, fn->sch, leader);

	free(rev);
	if (!ok) {
		sch_free(&hist->sch_ctx);
		return NULL;
	}

	if (!leader)
		leader = &sch_file;

	struct file lib_files[fn->n_libs];

	lib_init(&hist->lib);
	libs_open = 0;
	for (i = 0; i != fn->n_libs; i++)
		if (lib_find_file(lib_files + libs_open, fn->libs[libs_open],
		    fn, leader))
			libs_open++;

	hist->pl = pl_parse_search(fn->pl, leader);
	/*
	 * We treat failing to load or parse the page layout as a "minor"
	 * failure and don't reject the revision just because of it.
	 */

	if (hist->vcs_hist) {
		hist->oids = alloc_type_n(void *, libs_open);
		hist->libs_open = libs_open;
		for (i = 0; i != libs_open; i++)
			hist->oids[i] = file_oid(lib_files + i);
		if (prev && prev->vcs_hist && prev->libs_open == libs_open) {
			for (i = 0; i != libs_open; i++)
				if (!file_oid_eq(hist->oids[i], prev->oids[i]))
					break;
			if (i == libs_open) {
				hist->lib.comps = prev->lib.comps;
				libs_cached = 1;
			}
		}
	}

	if (!libs_cached)
		for (i = 0; i != libs_open; i++)
			if (!lib_parse_file(&hist->lib, lib_files +i))
				goto fail;

	if (!sch_parse(&hist->sch_ctx, &sch_file, &hist->lib,
	    libs_cached ? &prev->sch_ctx : NULL))
		goto fail;

	for (i = 0; i != libs_open; i++)
		file_close(lib_files + i);
	file_close(&sch_file);
	// @@@ close pro_file

	/*
	 * sheet_eq recurses into sub-sheets, so we compare all sheets, even if
	 * it may look like it.
	 */

	if (prev && prev->sheets &&
	    sheet_eq(prev->sch_ctx.sheets, hist->sch_ctx.sheets, 1))
		prev->identical = 1;

	/*
	 * @@@ we have a major memory leak for the component library.
	 * We should record parsed schematics and libraries separately, so
	 * that we can clean them up, without having to rely on the history,
	 * with - when sharing unchanged item - possibly many duplicate
	 * pointers.
	 */
	return hist->sch_ctx.sheets;

fail:
	while (libs_open--)
		file_close(lib_files + libs_open);
	sch_free(&hist->sch_ctx);
	lib_free(&hist->lib);
	file_close(&sch_file);
	// @@@ close pro_file
	return NULL;
}


struct add_hist_ctx {
	struct gui *gui;
	const struct file_names *fn;
	bool recurse;
	unsigned limit;
};


static void add_hist(void *user, struct vcs_hist *h,
    const struct vcs_hist *next)
{
	struct add_hist_ctx *ahc = user;
	struct gui *gui = ahc->gui;
	struct gui_hist **anchor, *hist, *prev;
	const struct sheet *sch;
	unsigned age = 0;

	if (!ahc->limit)
		return;
	if (ahc->limit > 0)
		ahc->limit--;

	prev = NULL;
	for (anchor = &gui->hist; *anchor; anchor = &(*anchor)->next) {
		prev = *anchor;
		age++;
	}

	/* @@@ should free the string returned by vcs_git_get_rev */
	progress(1, "processing revision %s",
	    h && h->commit ? vcs_git_get_rev(h) : "(uncommitted)");

	hist = alloc_type(struct gui_hist);
	hist->gui = gui;
	hist->vcs_hist = h;
	hist->libs_open = 0;
	hist->identical = 0;
	hist->pl = NULL;
	sch = parse_files(hist, ahc->fn, ahc->recurse, prev);
	hist->sheets = sch ? get_sheets(gui, hist, sch) : NULL;
	hist->age = age;

	hist->next = NULL;
	*anchor = hist;

	if (gui->hist_size)
		progress_update(gui);
}


static void get_revisions(struct gui *gui, const struct file_names *fn,
    bool recurse, int limit)
{
	struct add_hist_ctx add_hist_ctx = {
		.gui		= gui,
		.fn		= fn,
		.recurse	= recurse,
		.limit		= limit ? limit < 0 ? -limit : limit : -1,
	};

	if (gui->vcs_history)
		hist_iterate(gui->vcs_history, add_hist, &add_hist_ctx);
	else
		add_hist(&add_hist_ctx, NULL, NULL);
}


/* ----- Retrieve and count history ---------------------------------------- */


static void count_history(void *user, struct vcs_hist *h,
    const struct vcs_hist *next)
{
	struct gui *gui = user;

	gui->hist_size++;
}


static void get_history(struct gui *gui, const char *name, int limit)
{
	if (!vcs_git_try(name)) {
		gui->vcs_history = NULL;
		return;
	}

	if (limit < 0)
		limit = -limit;
	gui->vcs_history = vcs_git_history(name, limit);
	hist_iterate(gui->vcs_history, count_history, gui);
	if (limit && gui->hist_size > limit)
		gui->hist_size = limit;
}


/* ----- Resizing ---------------------------------------------------------- */


static void size_allocate_event(GtkWidget *widget, GdkRectangle *allocation,
    gpointer data)
{
	struct gui *gui = data;

	zoom_to_extents(gui);
	if (gui->mode == showing_index)
		index_resize(gui);
}


/* ----- Initialization ---------------------------------------------------- */


int run_gui(const struct file_names *fn, bool recurse, int limit,
    const char **commands, unsigned n_commands)
{
	GtkWidget *window;
	char *title;
	GtkSettings *settings = gtk_settings_get_default();
	struct gui gui = {
		.scale		= 1 / 16.0,
		.hist		= NULL,
		.vcs_history	= NULL,
		.mode		= showing_sheet,
		.sheet_overlays	= NULL,
		.hist_overlays	= NULL,
		.thumb_overlays	= NULL,
		.pop_overlays	= NULL,
		.pop_underlays	= NULL,
		.pop_origin	= NULL,
		.glabel		= NULL,
		.aois		= NULL,
		.diff_mode	= diff_delta,
		.old_hist	= NULL,
		.hist_y_offset	= 0,
		.commit_hover	= NULL,
		.hist_size	= 0,
	};

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gui.da = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), gui.da);

	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	alloc_printf(&title, "eeshow (rev %s)", version);
	gtk_window_set_title(GTK_WINDOW(window), title);

	gtk_widget_set_events(gui.da,
	    GDK_EXPOSE | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);

	input_setup(gui.da);

	gtk_widget_show_all(window);

	get_history(&gui, fn->pro ? fn->pro : fn->sch, limit);
	if (gui.hist_size)
		setup_progress_bar(&gui, window);

	get_revisions(&gui, fn, recurse, limit);
	for (gui.new_hist = gui.hist; gui.new_hist && !gui.new_hist->sheets;
	    gui.new_hist = gui.new_hist->next);
	if (!gui.new_hist)
		fatal("no valid sheets\n");

	g_signal_connect(G_OBJECT(gui.da), "size_allocate",
	    G_CALLBACK(size_allocate_event), &gui);
	g_signal_connect(window, "destroy",
	    G_CALLBACK(gtk_main_quit), NULL);

	icons_init();
	sheet_setup(&gui);
	render_setup(&gui);

//	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	/* http://gtk.10911.n7.nabble.com/Question-on-gtk-label-set-selectable-td9717.html */
	if (settings)
		g_object_set(settings, "gtk-label-select-on-focus",
		    FALSE, NULL);

	go_to_sheet(&gui, gui.new_hist->sheets);
	gtk_widget_show_all(window);

	if (n_commands) {
		long xid =
		    gdk_x11_window_get_xid(gtk_widget_get_window(window));
		char *s;

		alloc_printf(&s, "0x%lx", xid);
		setenv("EESHOW_WINDOW_ID", s, 1);

		while (n_commands--) {
			while (gtk_events_pending())
				gtk_main_iteration();

			system(*commands);
			commands++;
		}
	}

	/* for performance testing, use -N-depth */
	if (limit >= 0)
		gtk_main();

	return 0;
}
