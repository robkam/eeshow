/*
 * gui/glabel.c - Global label pop-up
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

#include <gtk/gtk.h>

#include "misc/util.h"
#include "kicad/dwg.h"
//#include "gui/input.h"
#include "gui/aoi.h"
#include "gui/over.h"
#include "gui/pop.h"
#include "gui/common.h"


struct glabel_aoi_ctx {
	const struct gui_sheet *sheet;
	const struct sch_obj *obj;
	struct dwg_bbox bbox;
	struct overlay *over;
};


#define	GLABEL_W	100


/* ----- AoIs -------------------------------------------------------------- */


static void glabel_dest_click(void *user)
{
	struct gui_sheet *sheet = user;

	go_to_sheet(sheet->gui, sheet);
}


static void add_dest_overlay(struct gui *gui, const char *label,
    struct gui_sheet *sheet, unsigned n)
{
	const struct sch_obj *obj;

	for (obj = sheet->sch->objs; obj; obj = obj->next) {
		if (obj->type != sch_obj_glabel)
			continue;
		if (strcmp(obj->u.text.s, label))
			continue;
		add_pop_item(gui, glabel_dest_click, sheet, GLABEL_W,
		    sheet == gui->curr_sheet, "%d %s", n,
		    sheet->sch->title ? sheet->sch->title : "(unnamed)");
		break;
	}
}


static bool hover_glabel(void *user, bool on, int dx, int dy)
{
	struct glabel_aoi_ctx *aoi_ctx = user;
	struct gui *gui = aoi_ctx->sheet->gui;

	if (!on) {
		dehover_pop(gui);
		return 1;
	}
	if (gui->pop_underlays) {
		if (gui->pop_origin == aoi_ctx)
			return 0;
		dehover_pop(gui);
	}

	unsigned n = 0;
	struct gui_sheet *sheet;

	gui->glabel = aoi_ctx->obj->u.text.s;
	gui->pop_origin = aoi_ctx;

	aoi_dehover();
	overlay_remove_all(&gui->pop_overlays);
	overlay_remove_all(&gui->pop_underlays);

	add_pop_header(gui, GLABEL_W, aoi_ctx->obj->u.text.s);
	for (sheet = gui->new_hist->sheets; sheet; sheet = sheet->next)
		add_dest_overlay(gui, aoi_ctx->obj->u.text.s, sheet, ++n);
	add_pop_frame(gui);

	place_pop(gui, &aoi_ctx->bbox);

	/*
	 * @@@ The idea is to get input to trigger hovering over the pop-up.
	 * However, this doesn't work because the overlay has not been drawn
	 * yet and therefore has not created its AoI. We therefore only get a
	 * chance to begin hovering at the next motion update, which may
	 * already be outside the pop-up.
	 *
	 * Probably the only way to fix this is by making overlay_add do the
	 * layout calculation and create the AoI immediately.
	 *
	 * Another problem occurs at deep zoom levels, when the label is larger
	 * than the pop-up. Then we can trigger pop-up creation from a location
	 * that will be outside the pop-up.
	 *
	 * We could fix this by aligning the pop-up with the mouse position
	 * instead the box, either in general, or in this specific case. Not
	 * sure if it's worth the trouble, though.
	 *
	 * Another way to avoid the problem would be to size the pop-up such
	 * that it always includes the mouse position. But that could lead to
	 * rather weird-looking results at deep zoom levels.
	 *
	 * Yet another option would be to move the mouse pointer onto the
	 * pop-up. The problem with this is that forced mouse pointer movement
	 * is not appreciated by all users.
	 *
	 * Both issues result in a "hanging" pop-up because AoI (and input)
	 * don't even know we're hovering. The pop-up can be cleared by
	 * - hovering into it,
	 * - hovering over some other glabel,
	 * - clicking, or
	 * - pressing Escape.
	 */

	/*
	 * @@@ Calling input_update here caused an infinite recursion through
	 * input_update -> hover_consider -> sheet_hover_update -> aoi_hover
	 * -> hover_d -> hover_glabel -> input_update -> etc.
	 * if we have overlapping global labels. The recursion altered between
	 * both AoIs.
	 */
//	input_update();
	redraw(gui);
	return 0;
}


void add_glabel_aoi(struct gui_sheet *sheet, const struct sch_obj *obj)
{
	const struct dwg_bbox *bbox = &obj->u.text.bbox;
	struct glabel_aoi_ctx *aoi_ctx = alloc_type(struct glabel_aoi_ctx);

	struct aoi cfg = {
		.x	= bbox->x,
		.y	= bbox->y,
		.w	= bbox->w,
		.h	= bbox->h,
		.hover	= hover_glabel,
		.user	= aoi_ctx,
	};

	aoi_ctx->sheet = sheet;
	aoi_ctx->obj = obj;
	aoi_ctx->bbox = *bbox;

	aoi_add(&sheet->aois, &cfg);
}
