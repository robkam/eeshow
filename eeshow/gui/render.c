/*
 * gui/render.c - Render schematics and GUI elements
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
#include <string.h>

#include <cairo/cairo.h>
#include <gtk/gtk.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/style.h"
#include "gfx/cro.h"
#include "gfx/gfx.h"
#include "kicad/pl.h"
#include "kicad/sch.h"
#include "kicad/delta.h"
#include "gfx/diff.h"
#include "gfx/diff.h"
#include "gui/aoi.h"
#include "gui/over.h"
#include "gui/timer.h"
#include "gui/common.h"


#define	SHEET_OVERLAYS_X	-10
#define	SHEET_OVERLAYS_Y	10

#define GLABEL_HIGHLIGHT_PAD	6


bool use_delta = 0;
enum gfx_extra show_extra = 0;


/* ----- Helper functions -------------------------------------------------- */


void redraw(const struct gui *gui)
{
	gtk_widget_queue_draw(gui->da);
}


/* ----- Highlight glabel -------------------------------------------------- */


/*
 * cd, cx, cy are simplified versions of what cro.c uses. Since we don't
 * support glabel highlighting in diff mode, we don't need the xe and ye offset
 * components.
 */

static inline int cd(int x, float scale)
{
	return x * scale;
}


static inline int cx(int x, int xo, float scale)
{
	return xo + x * scale;
}


static inline int cy(int y, int yo, float scale)
{
	return yo + y * scale;
}


static void highlight_glabel(const struct gui *gui,
    const struct gui_sheet *sheet,  cairo_t *cr, int xo, int yo, float f)
{
	const struct sch_obj *obj;

	if (!gui->glabel)
		return;

	cairo_set_source_rgb(cr, 1, 0.8, 1);
	for (obj = sheet->sch->objs; obj; obj = obj->next) {
		const struct dwg_bbox *bbox = &obj->u.text.bbox;

		if (obj->type != sch_obj_glabel)
			continue;
		if (strcmp(obj->u.text.s, gui->glabel))
			continue;

		cairo_rectangle(cr,
		    cx(bbox->x, xo, f) - GLABEL_HIGHLIGHT_PAD,
		    cy(bbox->y, yo, f) - GLABEL_HIGHLIGHT_PAD,
		    cd(bbox->w, f) + 2 * GLABEL_HIGHLIGHT_PAD,
		    cd(bbox->h, f) + 2 * GLABEL_HIGHLIGHT_PAD);
		cairo_fill(cr);
	}
}


/* ----- Draw to screen ---------------------------------------------------- */


/*
 * @@@ the highlighting of sub-sheets possibly containing changes is very
 * unreliable since sheet_eq (from delta) responds to a lot of purely
 * imaginary changes. However, this will be a good way to exercise and improve
 * delta.
 */

static struct area *changed_sheets(const struct gui *gui,
    int xo, int yo, float f)
{
	const struct gui_sheet *new = gui->curr_sheet;
	const struct sch_obj *obj;
	struct area *areas = NULL;

	for (obj = new->sch->objs; obj; obj = obj->next) {
		const struct gui_sheet *new_sub;
		const struct gui_sheet *old_sub;

		if (obj->type != sch_obj_sheet)
			continue;
		if (!obj->u.sheet.sheet)
			continue;

		for (new_sub = gui->new_hist->sheets;
		    new_sub && new_sub->sch != obj->u.sheet.sheet;
		    new_sub = new_sub->next);
		if (!new_sub)
			continue;
		old_sub = find_corresponding_sheet(gui->old_hist->sheets,
		    gui->new_hist->sheets, new_sub);

		if (!sheet_eq(new_sub->sch, old_sub->sch, 1))
			add_area(&areas, cx(obj->x, xo, f), cy(obj->y, yo, f),
			    cx(obj->x + obj->u.sheet.w, xo, f),
			    cy(obj->y + obj->u.sheet.h, yo, f), 0xffff00);
	}
	return areas;
}


static void hack(const struct gui *gui, cairo_t *cr,
    int xo, int yo, float f)
{
	const struct gui_sheet *new = gui->curr_sheet;
	const struct gui_sheet *old = find_corresponding_sheet(
	    gui->old_hist->sheets, gui->new_hist->sheets, gui->curr_sheet);
	struct area *areas = NULL;

	areas = changed_sheets(gui, xo, yo, f);
	diff_to_canvas(cr, gui->x, gui->y, gui->scale,
	    gfx_user(old->gfx), gfx_user(new->gfx), show_extra, areas);
	free_areas(&areas);
}


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
    gpointer user_data)
{
	struct gui *gui = user_data;
	const struct gui_sheet *sheet = gui->curr_sheet;
	GtkAllocation alloc;
	float f = gui->scale;
	int x, y;

	gtk_widget_get_allocation(gui->da, &alloc);
	x = -(sheet->xmin + gui->x) * f + alloc.width / 2;
	y = -(sheet->ymin + gui->y) * f + alloc.height / 2;

	cro_canvas_prepare(cr);
	if (!gui->old_hist || gui->diff_mode == diff_new) {
		highlight_glabel(gui, sheet, cr, x, y, f);
		cro_canvas_draw(gfx_user(sheet->gfx), cr, x, y, f, show_extra);
	} else if (gui->diff_mode == diff_old) {
		sheet = find_corresponding_sheet(gui->old_hist->sheets,
		    gui->new_hist->sheets, gui->curr_sheet);
		highlight_glabel(gui, sheet, cr, x, y, f);
		cro_canvas_draw(gfx_user(sheet->gfx), cr, x, y, f, show_extra);
	} else if (use_delta) {
		struct area *areas = changed_sheets(gui, x, y, f);
		const struct area *area;

		cairo_set_source_rgb(cr, 1, 1, 0);
		for (area = areas; area; area = area->next) {
			cairo_rectangle(cr, area->xa, area->ya,
			    area->xb - area->xa, area->yb - area->ya);
			cairo_fill(cr);
		}
		free_areas(&areas);

		/* @@@ fix geometry later */
		cro_canvas_draw(gfx_user(gui->delta_ab.gfx), cr, x, y, f,
		    show_extra);
		cro_canvas_draw(gfx_user(gui->delta_a.gfx), cr, x, y, f,
		    show_extra);
		cro_canvas_draw(gfx_user(gui->delta_b.gfx), cr, x, y, f,
		    show_extra);
	} else {
		hack(gui, cr, x, y, f);
	}

	overlay_draw_all(gui->sheet_overlays, cr,
	    SHEET_OVERLAYS_X, SHEET_OVERLAYS_Y);
	if (gui->mode != showing_history)
		overlay_draw_all_d(gui->hist_overlays, cr,
		    VCS_OVERLAYS_X, VCS_OVERLAYS_Y, 0, 1);
	overlay_draw_all_d(gui->pop_underlays, cr, gui->pop_x, gui->pop_y,
	    gui->pop_dx, gui->pop_dy);
	overlay_draw_all_d(gui->pop_overlays, cr,
	    gui->pop_x + gui->pop_dx * POP_STACK_PADDING,
	    gui->pop_y + gui->pop_dy * POP_STACK_PADDING,
	    gui->pop_dx, gui->pop_dy);

	switch (gui->mode) {
	case showing_history:
		history_draw_event(gui, cr);
		break;
	case showing_index:
		index_draw_event(gui, cr);
		break;
	default:
		break;
	}

	timer_show(cr);

	return FALSE;
}


/* ----- Pre-rendering ----------------------------------------------------- */


void render_sheet(struct gui_sheet *sheet)
{
	sheet->gfx = gfx_init(&cro_canvas_ops);
	if (sheet->hist && sheet->hist->pl) /* @@@ no pl_render for delta */
		pl_render(sheet->hist->pl, sheet->gfx,
		    sheet->hist->sch_ctx.sheets, sheet->sch);
	sch_render(sheet->sch, sheet->gfx);
	cro_canvas_end(gfx_user(sheet->gfx),
	    &sheet->w, &sheet->h, &sheet->xmin, &sheet->ymin);

	sheet->rendered = 1;
	// gfx_end();
}


void render_delta(struct gui *gui)
{
#if 1
	/* @@@ needs updating for curr/last vs. new/old */
	struct sheet *sch_a, *sch_b, *sch_ab;
	struct gui_sheet *a = gui->curr_sheet;
	struct gui_sheet *b = find_corresponding_sheet(
	    gui->old_hist->sheets, gui->new_hist->sheets, gui->curr_sheet);

	sch_a = alloc_type(struct sheet);
	sch_b = alloc_type(struct sheet);
	sch_ab = alloc_type(struct sheet);

	delta(a->sch, b->sch, sch_b, sch_a, sch_ab);
	gui->delta_a.sch = sch_a,
	gui->delta_b.sch = sch_b,
	gui->delta_ab.sch = sch_ab,

	gui->delta_a.gui = gui->delta_b.gui = gui->delta_ab.gui = NULL;
	gui->delta_a.hist = gui->delta_b.hist = gui->delta_ab.hist = NULL;

	render_sheet(&gui->delta_a);
	render_sheet(&gui->delta_b);
	render_sheet(&gui->delta_ab);

	cro_color_override(gfx_user(gui->delta_ab.gfx), COLOR_LIGHT_GREY);
	cro_color_override(gfx_user(gui->delta_a.gfx), COLOR_RED);
	cro_color_override(gfx_user(gui->delta_b.gfx), COLOR_GREEN2);

	// @@@ clean up when leaving sheet
#endif

	if (!b->rendered) {
		render_sheet(b);
		mark_aois(gui, b);
	}
}


/* ----- Setup ------------------------------------------------------------- */


void render_setup(struct gui *gui)
{
	g_signal_connect(G_OBJECT(gui->da), "draw",
	    G_CALLBACK(on_draw_event), gui);
}
