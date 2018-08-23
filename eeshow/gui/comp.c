/*
 * gui/comp.c - Component pop-up
 *
 * Written 2016-2017 by Werner Almesberger
 * Copyright 2016-2017 by Werner Almesberger
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
#include "misc/diag.h"
#include "db/doc.h"
#include "kicad/dwg.h"
#include "gfx/misc.h"
#include "gui/aoi.h"
#include "gui/over.h"
#include "gui/pop.h"
#include "gui/viewer.h"
#include "gui/common.h"


struct comp_pop_item {
	struct comp_aoi_ctx *ctx;
	const char *tag;
	const char *s;
	struct comp_pop_item *next;
};

struct comp_aoi_ctx {
	struct gui * gui;
	const struct sch_obj *obj;
	struct dwg_bbox bbox;
	struct overlay *over;
	const char *ref;
	const char *doc;		/* may be NULL */
	struct comp_pop_item *items;
};


#define	COMP_W	100

unsigned comp_pop_width = COMP_W;


/* ----- Bounding box ------------------------------------------------------ */


static void bbox_add(int x, int y, int *xa, int *ya, int *xb, int *yb,
    bool *first)
{
	if (x < *xa || *first)
		*xa = x;
	if (x > *xb || *first)
		*xb = x;
	if (y < *ya || *first)
		*ya = y;
	if (y > *yb || *first)
		*yb = y;
	*first = 0;
}


static void bbox_add_circ(int x, int y, int r,
    int *xa, int *ya, int *xb, int *yb, bool *first)
{
	bbox_add(x - r, y - r, xa, ya, xb, yb, first);
	bbox_add(x + r, y + r, xa, ya, xb, yb, first);
}


static struct dwg_bbox get_bbox(const struct sch_obj *sch_obj)
{
	const struct lib_obj *obj;
	int xa = 0, ya = 0, xb = 0, yb = 0;
	bool first = 1;
	int i;
	struct dwg_bbox bbox;
	const int *m = sch_obj->u.comp.m;

	for (obj = sch_obj->u.comp.comp->objs; obj; obj = obj->next) {
		if (obj->unit && sch_obj->u.comp.unit != obj->unit)
			continue;
		switch (obj->type) {
		case lib_obj_poly:
			for (i = 0; i != obj->u.poly.points; i++)
				bbox_add(obj->u.poly.x[i], obj->u.poly.y[i],
				    &xa, &ya, &xb, &yb, &first);
			break;
		case lib_obj_rect:
			bbox_add(obj->u.rect.sx, obj->u.rect.sy,
			    &xa, &ya, &xb, &yb, &first);
			bbox_add(obj->u.rect.ex, obj->u.rect.ey,
			    &xa, &ya, &xb, &yb, &first);
			break;
		case lib_obj_circ:
			bbox_add_circ(obj->u.circ.x, obj->u.circ.y,
			    obj->u.circ.r, &xa, &ya, &xb, &yb, &first);
			break;
		case lib_obj_arc:
			bbox_add_circ(obj->u.arc.x, obj->u.arc.y,
			    obj->u.arc.r, &xa, &ya, &xb, &yb, &first);
			break;
		/* @@@ consider pins, too ? */
		default:
			break;
		}
	}

	bbox.x = mx(xa, ya, m);
	bbox.y = my(xa, ya, m);
	bbox.w = mx(xb, yb, m) - bbox.x + 1;
	bbox.h = my(xb, yb, m) - bbox.y + 1;
	if (bbox.w < 0) {
		bbox.w = -bbox.w;
		bbox.x -= bbox.w;
	}
	if (bbox.h < 0) {
		bbox.h = -bbox.h;
		bbox.y -= bbox.h;
	}

	return bbox;
}


/* ----- AoIs -------------------------------------------------------------- */


static void comp_click(void *user)
{
	const struct comp_pop_item *item = user;
	const struct comp_aoi_ctx *ctx = item->ctx;
	struct gui *gui = ctx->gui;

	viewer(item->s);
	dehover_pop(gui);
}


static bool hover_comp(void *user, bool on, int dx, int dy)
{
	struct comp_aoi_ctx *ctx = user;
	struct gui *gui = ctx->gui;
	const struct comp_pop_item *item;

	if (!on) {
		dehover_pop(gui);
		return 1;
	}
	if (gui->pop_underlays) {
		if (gui->pop_origin == ctx)
			return 0;
		dehover_pop(gui);
	}

	gui->pop_origin = ctx;

	aoi_dehover();
	overlay_remove_all(&gui->pop_overlays);
	overlay_remove_all(&gui->pop_underlays);

	add_pop_header(gui, comp_pop_width, ctx->ref);
	for (item = ctx->items; item; item = item->next)
		add_pop_item(gui, comp_click, (void *) item, comp_pop_width, 0,
		    "%s", item->tag);
	add_pop_frame(gui);

	place_pop(gui, &ctx->bbox);

	redraw(gui);
	return 0;
}


/* ----- Setup ------------------------------------------------------------- */


static void add_item(struct comp_aoi_ctx *ctx, const char *tag, const char *s)
{
	struct comp_pop_item **item;

	for (item = &ctx->items; *item; item = &(*item)->next);
	*item = alloc_type(struct comp_pop_item);
	(*item)->ctx = ctx;
	(*item)->tag = tag;
	(*item)->s = s;
	(*item)->next = NULL;
}


static void add_doc(void *user, const char *tag, const char *s)
{
	struct comp_aoi_ctx *ctx = user;

	/* avoid duplication */
	if (ctx->doc && !strcmp(ctx->doc, s))
		ctx->doc = NULL;
	add_item(ctx, tag, s);
}


void add_comp_aoi(struct gui_sheet *sheet, const struct sch_obj *obj)
{
	struct dwg_bbox bbox;
	struct comp_aoi_ctx *ctx = alloc_type(struct comp_aoi_ctx);
	const struct comp_field *f;

	if (!obj->u.comp.comp)
		return;

	bbox = get_bbox(obj);

	struct aoi cfg = {
		.x	= bbox.x,
		.y	= bbox.y,
		.w	= bbox.w,
		.h	= bbox.h,
		.hover	= hover_comp,
		.user	= ctx,
	};

	ctx->gui = sheet->gui;
	ctx->obj = obj;
	ctx->bbox = bbox;
	ctx->ref = NULL;
	ctx->items = NULL;
	ctx->doc = NULL;

	for (f = obj->u.comp.fields; f; f = f->next)
		switch (f->n) {
		case 0:
			/* ignore power symbols */
			if (*f->txt.s == '#') {
				free(ctx);
				return;
			}
			ctx->ref = f->txt.s;
			break;
		case 3:
			if (strcmp(f->txt.s, "~"))
				ctx->doc = f->txt.s;
			break;
		default:
			break;
		}

	if (!ctx->ref)
		BUG("component without reference");
	doc_find(ctx->ref, add_doc, ctx);
	if (ctx->doc)
		add_item(ctx, "Doc", ctx->doc);
	
	aoi_add(&sheet->aois, &cfg);
}
