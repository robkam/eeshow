/*
 * gui/index.c - Sheet index
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdbool.h>
#include <assert.h>

#include <gtk/gtk.h>

#include "gfx/record.h"
#include "kicad/delta.h"
#include "gui/aoi.h"
#include "gui/style.h"
#include "gui/over.h"
#include "gui/input.h"
#include "gui/help.h"
#include "gui/common.h"


#define	SHEET_MAX_W	240
#define	SHEET_MAX_NAME	300
#define	SHEET_ASPECT	1.4146	/* width / height */
#define	SHEET_PAD	3
#define	SHEET_GAP	12	/* not counting the padding ! */
#define	INDEX_MARGIN	10	/* margin on each side */
#define	LABEL_GAP	6	/* gap between name and number */


/* @@@ clean all this up and move into struct gui */
static unsigned thumb_rows, thumb_cols;
static unsigned thumb_w, thumb_h;
static struct overlay *name_over = NULL;
static struct overlay *number_over = NULL;
static const struct gui_sheet *curr_sheet = NULL;


/* ----- Tools ------------------------------------------------------------- */


static void thumbnail_pos(GtkAllocation *alloc, unsigned n, int *ix, int *iy)
{
	*ix = alloc->width / 2 + (thumb_w + SHEET_GAP) *
	    (n % thumb_cols - (thumb_cols - 1) / 2.0);
	*iy = alloc->height / 2 + (thumb_h + SHEET_GAP) *
	    (n / thumb_cols - (thumb_rows - 1) / 2.0);
}


/* ----- Drawing ----------------------------------------------------------- */


static int center_x(const struct overlay *over, int w, int x, int width)
{
	x -= w / 2;
	if (x < INDEX_MARGIN)
		x = INDEX_MARGIN;
	if (x + w >= width - INDEX_MARGIN)
		x = width - w - INDEX_MARGIN;
	return x;
}


void index_draw_event(const struct gui *gui, cairo_t *cr)
{
	GtkAllocation alloc;
	const struct gui_sheet *sheet;
	unsigned n = 0;
	int ix, iy, x, y;
	int name_w, name_h, number_w, number_h;
	int named = -1;

	gtk_widget_get_allocation(gui->da, &alloc);

	cairo_set_source_rgba(cr, 1, 1, 1, 0.7);
	cairo_paint(cr);

	n = 0;
	for (sheet = sheets(gui); sheet; sheet = sheet->next) {
		thumbnail_pos(&alloc, n, &ix, &iy);
		x = ix - thumb_w / 2 - SHEET_PAD;
		y = iy - thumb_h / 2 - SHEET_PAD;

		overlay_draw(sheet->thumb_over, cr, x, y, 1, 1);
		if (name_over && curr_sheet == sheet)
			named = n;
		n++;
	}

	if (named == -1)
		return;

	thumbnail_pos(&alloc, named, &ix, &iy);
	overlay_size(name_over, gtk_widget_get_pango_context(gui->da),
	    &name_w, &name_h);
	overlay_size(number_over, gtk_widget_get_pango_context(gui->da),
	    &number_w, &number_h);

	/* center overlay pair for name and number on thumbnail */
	y = iy + (name_h - number_h) / 2;
	if (y - name_h - LABEL_GAP / 2 < INDEX_MARGIN)
		y = name_h + LABEL_GAP / 2 + INDEX_MARGIN;
	if (y + number_h + LABEL_GAP / 2 >= alloc.height - INDEX_MARGIN)
		y = alloc.height - number_h - LABEL_GAP / 2 - INDEX_MARGIN;

	overlay_draw(name_over, cr,
	    center_x(name_over, name_w, ix, alloc.width), y - LABEL_GAP / 2,
	    1, -1);
	overlay_draw(number_over, cr,
	    center_x(number_over, number_w, ix, alloc.width), y + LABEL_GAP / 2,
	    1, 1);
}


/* ----- Thumbnail actions ------------------------------------------------- */


static void close_index(struct gui *gui)
{
	overlay_remove_all(&gui->thumb_overlays);
	name_over = number_over = NULL;
	gui->mode = showing_sheet;
	input_pop();
	redraw(gui);
}


static void thumb_click(void *user)
{
	struct gui_sheet *sheet = user;
	struct gui *gui = sheet->gui;

	go_to_sheet(gui, sheet);
	close_index(gui);
}


static void thumb_set_style(struct gui_sheet *sheet, bool selected)
{
	struct overlay_style style = overlay_style_dense;

	style.radius = 3;
	style.pad = SHEET_PAD;
	style.bg = RGBA(1, 1, 1, 0.8);

	if (selected) {
		style.width = 2;
		style.frame = RGBA(0, 0, 0, 1);
		style.bg = RGBA(1, 1, 1, 1);
	}

	if (sheet->thumb_yellow)
		style.bg = RGBA(1.0, 1.0, 0, 1);

	overlay_style(sheet->thumb_over, &style);
}


static unsigned sheet_number(const struct gui *gui, struct gui_sheet *sheet)
{
	const struct gui_sheet *s;
	unsigned n = 1;

	for (s = sheets(gui); s; s = s->next) {
		if (s == sheet)
			return n;
		n++;
	}
	return 0; /* should never happen */
}


static bool thumb_hover(void *user, bool on, int dx, int dy)
{
	struct gui_sheet *sheet = user;
	struct gui *gui = sheet->gui;
	struct overlay_style style = overlay_style_default;
	const char *file;

	if (on) {
		thumb_set_style(sheet, 1);

		name_over = overlay_add(&gui->thumb_overlays, &gui->aois,
		    NULL, NULL, NULL);
		file = sheet->sch->file;
		if (sheet->sch && sheet->sch->title)
			overlay_text(name_over, "%s<small>%s%s</small>",
			    sheet->sch->title,
			    file ? "\n" : "", file ? file : "");
		else
			overlay_text(name_over, "%s", file ? file : "");

		number_over = overlay_add(&gui->thumb_overlays, &gui->aois,
		    NULL, NULL, NULL);
		overlay_text(number_over, "<big>%u</big>",
		    sheet_number(gui, sheet));

		style.font = BOLD_FONT_LARGE;
		style.width = 1;
		style.wmax = SHEET_MAX_NAME;
		overlay_style(name_over, &style);
		overlay_style(number_over, &style);

		curr_sheet = sheet;
	} else {
		thumb_set_style(sheet, 0);
		overlay_remove(&gui->thumb_overlays, name_over);
		overlay_remove(&gui->thumb_overlays, number_over);
		name_over = number_over = NULL;
	}
	redraw(gui);
	return 1;
}


/* ----- Rendering to cache ------------------------------------------------ */


static bool best_ratio(const struct gui *gui)
{
	GtkAllocation alloc;
	const struct gui_sheet *sheet;
	unsigned n = 0;
	unsigned r, c;
	float size, best_size = 0;
	int aw, ah;	/* available size */
	int w, h;

	gtk_widget_get_allocation(gui->da, &alloc);

	for (sheet = sheets(gui); sheet; sheet = sheet->next)
		n++;
	assert(n);

	for (r = 1; r <= n; r++) {
		c = (n + r - 1) / r;
		aw = alloc.width - (c - 1) * SHEET_GAP - 2 * INDEX_MARGIN;
		ah = alloc.height - (r - 1) * SHEET_GAP - 2 * INDEX_MARGIN;
		if (aw < 0 || ah < 0)
			continue;
		w = aw / c;
		h = ah / r;
		if (w > SHEET_MAX_W)
			w = SHEET_MAX_W;
		if (h * SHEET_ASPECT > w)
			 h = w / SHEET_ASPECT;
		if (w / SHEET_ASPECT > h)
			w = h * SHEET_ASPECT;
		if (!w || !h)
			continue;
		size = ((c - 1) * (w + SHEET_GAP) + w) *
		    ((r - 1) * (h + SHEET_GAP) + h);
		if (size > best_size) {
			best_size = size;
			thumb_cols = c;
			thumb_rows = r;
			thumb_w = w;
			thumb_h = h;
		}
	}

	return best_size;
}


static void index_render_sheet(const struct gui *gui,
    struct gui_sheet *sheet)
{
	int xmin, ymin, w, h;
	float fw, fh, f;
	bool yellow = 0;
	int xo, yo;
	int stride;

	if (!sheet->gfx_thumb) {
		sheet->gfx_thumb = gfx_init(&cro_canvas_ops);
		sch_render(sheet->sch, sheet->gfx_thumb);
		cro_canvas_end(gfx_user(sheet->gfx_thumb),
		    NULL, NULL, NULL, NULL);
	}

	if (gui->old_hist && gui->diff_mode == diff_delta) {
		const struct gui_sheet *old;

		old = find_corresponding_sheet(gui->old_hist->sheets,
		    gui->new_hist->sheets, sheet);
		if (!sheet_eq(sheet->sch, old->sch, 0))
			yellow = 1;
	}

	if (sheet->thumb_surf &&
	    sheet->thumb_w == thumb_w && sheet->thumb_h == thumb_h &&
	    sheet->thumb_yellow == yellow)
		return;

	if (sheet->thumb_surf) {
		// @@@ free data
		cairo_surface_destroy(sheet->thumb_surf);
		sheet->thumb_surf = NULL;
	}

	record_bbox((const struct record *) gfx_user(sheet->gfx_thumb),
	    &xmin, &ymin, &w, &h);
	if (!w || !h)
		return;

	fw = (float) thumb_w / w;
	fh = (float) thumb_h / h;
	f = fw < fh ? fw : fh;

	xo = -(xmin + w / 2) * f + thumb_w / 2;
	yo = -(ymin + h / 2) * f + thumb_h / 2;
	cro_img(gfx_user(sheet->gfx_thumb), 0,
	    xo, yo, thumb_w, thumb_h,  f, 0, NULL, &stride);

	sheet->thumb_surf = cro_img_surface(gfx_user(sheet->gfx_thumb));
	sheet->thumb_w = thumb_w;
	sheet->thumb_h = thumb_h;
	sheet->thumb_yellow = yellow;
}


static void index_add_overlay(struct gui *gui, struct gui_sheet *sheet)
{
	sheet->thumb_over = overlay_add(&gui->thumb_overlays, &gui->aois,
	    thumb_hover, thumb_click, sheet);
	overlay_icon(sheet->thumb_over, sheet->thumb_surf);
	thumb_set_style(sheet, 0);
}


static void index_render_sheets(struct gui *gui)
{
	struct gui_sheet *sheet;

	for (sheet = sheets(gui); sheet; sheet = sheet->next) {
		index_render_sheet(gui, sheet);
		index_add_overlay(gui, sheet);
	}
}


/* ----- Input ------------------------------------------------------------- */


static bool index_hover_update(void *user, int x, int y)
{
	struct gui *gui = user;

	if (aoi_hover(&gui->aois, x, y))
		return 1;
	return 0;
}


static bool index_click(void *user, int x, int y)
{
	struct gui *gui = user;

	if (aoi_click(&gui->aois, x, y))
		return 1;
	close_index(gui);
	return 1;
}


static void index_key(void *user, int x, int y, int keyval)
{
	struct gui *gui = user;

	switch (keyval) {
	case GDK_KEY_Escape:
		gui->mode = showing_sheet;
		input_pop();
		redraw(gui);
		break;

	case GDK_KEY_h:
	case GDK_KEY_Help:
		help();
		break;

	case GDK_KEY_q:
		gtk_main_quit();
	}
}


static const struct input_ops index_input_ops = {
	.click		= index_click,
	.hover_begin	= index_hover_update,
	.hover_update	= index_hover_update,
	.hover_click	= index_click,
	.key		= index_key,
};


/* ----- Resizing ---------------------------------------------------------- */


void index_resize(struct gui *gui)
{
	overlay_remove_all(&gui->thumb_overlays);
	name_over = number_over = NULL;
	if (best_ratio(gui))
		index_render_sheets(gui);
	else
		close_index(gui);
	redraw(gui);
}


/* ----- Initialization ---------------------------------------------------- */


void show_index(struct gui *gui)
{
	input_push(&index_input_ops, gui);
	gui->mode = showing_index;
	index_resize(gui);
}
