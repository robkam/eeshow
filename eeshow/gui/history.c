/*
 * gui/history.c - Revision history (navigation)
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include <gtk/gtk.h>
#include <pango/pangocairo.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/git-hist.h"
#include "gui/fmt-pango.h"
#include "gui/view.h"
#include "gui/style.h"
#include "gui/aoi.h"
#include "gui/over.h"
#include "gui/input.h"
#include "gui/help.h"
#include "gui/common.h"


/* ----- Commit view ------------------------------------------------------- */


static struct view *view = NULL;


static void key_press(void *user, struct view *v, guint keyval)
{
	switch (keyval) {
	case GDK_KEY_v:
	case GDK_KEY_q:
	case GDK_KEY_Escape:
		view_visible(v, 0);
		break;
	}
}


void commit_hover(struct gui *gui, const struct vcs_hist *vcs_hist)
{
	char *s;

	gui->commit_hover = vcs_hist;
	/* should only update if visible */
	if (view && vcs_hist) {
		s = vcs_git_long_for_pango(vcs_hist, fmt_pango, 1);
		view_update(view, s, 1);
		free(s);
	}
}


void view_full_commit(struct gui *gui)
{
	if (!view || !view_visible_toggle(view)) {
		if (view)
			view_close(view);
		view = view_open(key_press, NULL);
	}
	commit_hover(gui, gui->commit_hover);
}


/* ----- Render threads ---------------------------------------------------- */


#define		THREAD_MARGIN	(VCS_OVERLAYS_X + 4)  /* offset padding */

#define		COMMIT_R	4
#define		CORNER_R	4
#define		THREAD_WIDTH	2
#define		GAP_Y		4

#define		THREAD_DX	12


static const struct color commit_rgb = { 0.2, 0.2, 0.9, 1 };
static const struct color thread_rgb = { 0.1, 0.4, 1.0, 1 };
static const struct color skipped_rgb = { 0.7, 0.7, 0.7, 1 };


static void do_render_commit(const struct gui *gui, struct gui_hist *h,
    cairo_t *cr, unsigned x, unsigned y, unsigned dx,
    unsigned d_up, unsigned d_down,
    const struct color *cc, const struct color *tc, bool top, bool bottom)
{
	unsigned n = threads_number(gui->vcs_history);
	enum thread *t = threads_classify(gui->vcs_history, h->vcs_hist,
	    h->next ? h->next->vcs_hist : NULL);
	unsigned from, to;
	unsigned i;

	cairo_set_line_width(cr, THREAD_WIDTH);

	for (from = 0; from != n; from++)
		if (t[from] & ~thread_other)
			break;
	for (i = 0; i != n; i++)
		if (t[i] & ~thread_other)
			to = i;
	for (i = 0; i != n; i++) {
		cairo_set_source_rgb(cr, tc->r, tc->g, tc->b);
		if (i >= from && i < to) {
			cairo_move_to(cr, x, y);
			cairo_rel_line_to(cr, dx, 0);
			cairo_stroke(cr);
		}
		if (top && (t[i] & thread_newer)) {
			cairo_move_to(cr, x, y);
			cairo_rel_line_to(cr, 0, -d_up);
			cairo_stroke(cr);
		}
		if (bottom && (t[i] & thread_older)) {
			cairo_move_to(cr, x, y);
			cairo_rel_line_to(cr, 0, d_down);
			cairo_stroke(cr);
		}
		if (t[i] & thread_other) {
			if (i >= from && i < to) {
				if (top) {
					cairo_move_to(cr, x, y - GAP_Y);
					cairo_rel_line_to(cr, 0, -d_up + GAP_Y);
				}
				if (bottom) {
					cairo_move_to(cr, x, y + GAP_Y);
					cairo_rel_line_to(cr,
					    0, d_down - GAP_Y);
				}
			} else {
				if (top && bottom) {
					cairo_move_to(cr, x, y - d_up);
					cairo_rel_line_to(cr, 0, d_up + d_down);
				} else if (top) {
					cairo_move_to(cr, x, y - d_up);
					cairo_rel_line_to(cr, 0, d_up);
				} else if (bottom) {
					cairo_move_to(cr, x, y);
					cairo_rel_line_to(cr, 0, d_down);
				}
			}
			cairo_stroke(cr);
		}
		if (t[i] & thread_self) {
			cairo_set_source_rgb(cr, cc->r, cc->g, cc->b);
			cairo_arc(cr, x, y, COMMIT_R, 0, 2 * M_PI);
			cairo_fill(cr);
		}
		x += dx;
	}
}


static void render_commit(void *user, void *user_over, int x, int y,
    unsigned w, unsigned h, int dy)
{
	struct gui_hist *hist = user_over;
	cairo_t *cr = user;
	struct gui_hist *walk;

	if (hist->skipped) {
		for (walk = hist; walk && walk->skipped; walk = walk->next)
			do_render_commit(walk->gui, walk, cr,
			    VCS_OVERLAYS_X + COMMIT_R,
			    y + h / 2, THREAD_DX, h / 2, dy - h  / 2,
			    &skipped_rgb, &skipped_rgb,
			    walk == hist, walk->next && !walk->next->skipped);
		return;
	}

	if (y + (int) h < 0)
		return;

	do_render_commit(hist->gui, hist, cr, VCS_OVERLAYS_X + COMMIT_R,
	    y + h / 2, THREAD_DX, h / 2, dy - h  / 2, &commit_rgb, &thread_rgb,
	    1, 1);
}


/* ----- Drawing ----------------------------------------------------------- */


void history_draw_event(const struct gui *gui, cairo_t *cr)
{
	cairo_set_source_rgba(cr, 1, 1, 1, 0.7);
	cairo_paint(cr);

	overlay_draw_all_d(gui->hist_overlays, cr,
	    VCS_OVERLAYS_X + THREAD_MARGIN +
	    (threads_number(gui->vcs_history) - 1) * THREAD_DX + 2 * COMMIT_R,
	    VCS_OVERLAYS_Y + gui->hist_y_offset, 0, 1);
	over_iterate_geometry(gui->hist_overlays, render_commit, cr);
}


/* ----- Overlay interaction ----------------------------------------------- */


static void hide_history(struct gui *gui)
{
	input_pop();

	gui->mode = showing_sheet;
	do_revision_overlays(gui);
	commit_hover(gui, NULL);
	redraw(gui);
}


static void set_history_style(struct gui_hist *h, bool current)
{
	struct gui *gui = h->gui;
	struct overlay_style style = overlay_style_dense;
	const struct gui_hist *new = gui->new_hist;
	const struct gui_hist *old = gui->old_hist;

	/* this is in addition to showing detailed content */
	if (current)
		style.width++;

	switch (gui->selecting) {
	case sel_only:
	case sel_split:
		style.frame = COLOR(FRAME_SEL_ONLY);
		break;
	case sel_old:
		style.frame = COLOR(FRAME_SEL_OLD);
		break;
	case sel_new:
		style.frame = COLOR(FRAME_SEL_NEW);
		break;
	default:
		BUG("invalid mode %d", gui->selecting);
	}

	if (gui->new_hist == h || gui->old_hist == h) {
		style.width++;
		style.font = BOLD_FONT;
	}
	if (gui->old_hist) {
		if (h == new)
			style.bg = COLOR(BG_NEW);
		if (h == old)
			style.bg = COLOR(BG_OLD);
	}

	if (h->identical)
		style.fg = RGBA(0.5, 0.5, 0.5, 1);
	if (!h->sheets)
		style.fg = RGBA(0.7, 0.0, 0.0, 1);

	overlay_style(h->over, &style);
}


/*
 * One difficulty with resizing (enlarging, in this case) list items on hover
 * is that, if we only change the size but not the position, hovering towards
 * the next item will cause the previous item to shrink and thus move the next
 * item up - possibly even above the mouse pointer. This can be confusing.
 *
 * We could adjust the mouse pointer, but manipulating the pointer position is
 * not universally popular.
 *
 * Instead, we move the list such that the bottom edge of the item we're
 * leaving remains stationary. Thus the list moves down when mousing over items
 * from the top towards the bottom.
 *
 * To prevent this movement from being overly pronounced, we try to predict the
 * direction in which an item will be left (i.e., in the same direction from
 * which it was entered), and compensate for the likely list movement on
 * departure on entry.
 *
 * These heuristics can still sometimes fail, but by and large, they produce
 * the desired result without introducing too much list movement.
 */

static bool hover_history(void *user, bool on, int dx, int dy)
{
	struct gui_hist *h = user;
	struct gui *gui = h->gui;
	char *s;
	int before, after;

	if (dy)
		overlay_size(h->over, gtk_widget_get_pango_context(gui->da),
		    NULL, &before);
	if (on) {
		s = vcs_git_long_for_pango(h->vcs_hist, fmt_pango, 0);
		commit_hover(gui, h->vcs_hist);
	} else {
		s = vcs_git_summary_for_pango(h->vcs_hist, fmt_pango);
		commit_hover(gui, NULL);
	}
	overlay_text_raw(h->over, s);
	free(s);
	set_history_style(h, on);
	if (dy)
		overlay_size(h->over, gtk_widget_get_pango_context(gui->da),
		    NULL, &after);

	if (dy < 0 && on)
		gui->hist_y_offset -= after - before;
	if (dy < 0 && !on && !h->age)
		gui->hist_y_offset -= after - before;
	if (dy > 0 && !on && h->next)
		gui->hist_y_offset -= after - before;

	redraw(gui);
	return 1;
}


static void click_history(void *user)
{
	struct gui_hist *h = user;
	struct gui *gui = h->gui;
	struct gui_sheet *sheet, *old_sheet;

	hide_history(gui);

	if (!h->sheets)
		return;

	sheet = find_corresponding_sheet(h->sheets,
	    gui->new_hist->sheets, gui->curr_sheet);
	old_sheet = find_corresponding_sheet(
	    gui->old_hist ? gui->old_hist->sheets : gui->new_hist->sheets,
	    gui->new_hist->sheets, gui->curr_sheet);

	switch (gui->selecting) {
	case sel_only:
		gui->new_hist = h;
		break;
	case sel_split:
		gui->old_hist = gui->new_hist;
		gui->new_hist = h;
		break;
	case sel_new:
		gui->new_hist = h;
		break;
	case sel_old:
		gui->old_hist = h;
		break;
	default:
		BUG("invalid mode %d", gui->selecting);
	}

	gui->diff_mode = diff_delta;

	if (gui->old_hist) {
		if (gui->new_hist->age > gui->old_hist->age) {
			swap(gui->new_hist, gui->old_hist);
			if (gui->selecting == sel_old) {
				go_to_sheet(gui, sheet);
			} else {
				go_to_sheet(gui, old_sheet);
				render_delta(gui);
			}
		} else {
			if (gui->selecting != sel_old)
				go_to_sheet(gui, sheet);
			else
				render_delta(gui);
		}
	} else {
		go_to_sheet(gui, sheet);
	}

	if (gui->old_hist == gui->new_hist)
		gui->old_hist = NULL;

	do_revision_overlays(gui);
	redraw(gui);
}


/* ----- Skip commits that don't change our schematics --------------------- */


static void ignore_click(void *user)
{
}


static struct gui_hist *skip_history(struct gui *gui, struct gui_hist *h)
{
	struct gui_hist *start = h;
	struct overlay_style style = overlay_style_dense;
	unsigned n;

	/* don't skip the first entry */
	if (h == gui->hist)
		return h;

	/* need at least two entries */
	if (!h->next || !h->next->identical)
		return h;

	/* don't skip the last entry */
	for (n = 0; h->next && h->identical && !h->vcs_hist->n_branches;
	    h = h->next) {
		h->skipped = 1;
		n++;
	}
	if (!n)
		return h;

	h->over = overlay_add(&gui->hist_overlays, &gui->aois,
	    NULL, ignore_click, start);
	overlay_text(h->over, "<small>%u commits without changes</small>", n);

	style.width = 0;
	style.pad = 0;
	style.bg = RGBA(1.0, 1.0, 1.0, 0.8);
	overlay_style(h->over, &style);

	return h;
}


/* ----- Input ------------------------------------------------------------- */


static bool history_click(void *user, int x, int y)
{
	struct gui *gui = user;

	if (aoi_click(&gui->aois, x, y))
		return 1;
	hide_history(gui);
	return 1;
}


static bool history_hover_update(void *user, int x, int y)
{
	struct gui *gui = user;

	return aoi_hover(&gui->aois, x, y);
}


static void history_drag_move(void *user, int dx, int dy)
{
	struct gui *gui = user;

	gui->hist_y_offset += dy;
	redraw(gui);
}


static void history_scroll(void *user, int x, int y, int dy)
{
	struct gui *gui = user;

	if (dy < 0) {
		gui->hist_y_offset += 20;
		if (gui->hist_y_offset > 0)
			gui->hist_y_offset = 0;
	} else {
		gui->hist_y_offset -= 20;
	}
	redraw(gui);
}


static void history_key(void *user, int x, int y, int keyval)
{
	struct gui *gui = user;

	switch (keyval) {
	case GDK_KEY_Escape:
		hide_history(gui);
		break;
	case GDK_KEY_h:
	case GDK_KEY_Help:
		help();
		break;
	case GDK_KEY_v:
		if (gui->commit_hover)
			view_full_commit(gui);
		break;
	case GDK_KEY_q:
		gtk_main_quit();
		break;
	}
}


static const struct input_ops history_input_ops = {
	.click		= history_click,
	.hover_begin	= history_hover_update,
	.hover_update	= history_hover_update,
	.hover_click	= history_click,
	.drag_begin	= input_accept,
	.drag_move	= history_drag_move,
	.scroll		= history_scroll,
	.key		= history_key,
};


/* ----- Invocation -------------------------------------------------------- */


void show_history(struct gui *gui, enum selecting sel)
{
	struct gui_hist *h = gui->hist;

	input_push(&history_input_ops, gui);

	gui->mode = showing_history;
	gui->hist_y_offset = 0;
	gui->selecting = sel;
	overlay_remove_all(&gui->hist_overlays);
	for (h = gui->hist; h; h = h->next) {
		h = skip_history(gui, h);
		h->over = overlay_add(&gui->hist_overlays, &gui->aois,
		    hover_history, click_history, h);
		h->skipped = 0;
		set_history_style(h, 0);
		hover_history(h, 0, 0, 0);
	}
	redraw(gui);
}
