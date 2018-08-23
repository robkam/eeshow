/*
 * gfx/record.c - Record graphics operations by layers and replay
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/style.h"
#include "gfx/gfx.h"
#include "gfx/text.h"
#include "gfx/record.h"


struct record_obj {
	enum ro_type {
		ro_line,
		ro_rect,
		ro_poly,
		ro_circ,
		ro_arc,
		ro_text,
	} type;

	enum gfx_extra extra;

	int x, y;
	int color, fill_color;
	union {
		struct {
			int ex, ey;
		} line;
		struct {
			int ex, ey;
		} rect;
		struct {
			unsigned n;
			int *vx, *vy;
		} poly;
		struct {
			int r;
		} circ;
		struct {
			int r;
			int sa, ea;
		} arc;
		struct {
			char *s;
			unsigned size;
			enum text_align align;
			int rot;
			enum text_style style;
			struct record_bbox bbox;
		} text;
	} u;
	struct record_obj *next;
};


/* ----- Helper functions -------------------------------------------------- */


static void bb_init(struct record_bbox *bbox)
{
	bbox->xmin = bbox->ymin = INT_MAX;
	bbox->xmax = bbox->ymax = INT_MIN;
}


static void bb(struct record_bbox *bbox, int x, int y)
{
	if (bbox->xmin > x)
		bbox->xmin = x;
	if (bbox->ymin > y)
		bbox->ymin = y;
	if (bbox->xmax < x)
		bbox->xmax = x;
	if (bbox->ymax < y)
		bbox->ymax = y;
}


static void bb_rot(struct record_bbox *bbox,
    int x, int y, int dx, int dy, int rot)
{
	double a = rot / 180.0 * M_PI;

	bb(bbox, x + cos(a) * dx + sin(a) * dy, y + cos(a) * dy - sin(a) * dx);
}


static struct record_obj *new_obj(struct record *rec, enum ro_type type,
    int color, int fill_color, unsigned layer)
{
	struct record_layer **curr_layer;
	struct record_layer *new_layer;
	struct record_obj *new_obj;

	for (curr_layer = &rec->layers; *curr_layer;
	    curr_layer= &(*curr_layer)->next) {
		if ((*curr_layer)->layer == layer)
			goto this_layer;
		if ((*curr_layer)->layer < layer)
			break;
	}

	new_layer = alloc_type(struct record_layer);
	new_layer->layer = layer;
	new_layer->objs = NULL;
	new_layer->next_obj = &new_layer->objs;
	new_layer->next = *curr_layer;
	*curr_layer = new_layer;

this_layer:
	new_obj = alloc_type(struct record_obj);
	new_obj->type = type;
	new_obj->extra = rec->extra;
	new_obj->color = color;
	new_obj->fill_color = fill_color;
	new_obj->next = NULL;

	*(*curr_layer)->next_obj = new_obj;
	(*curr_layer)->next_obj = &new_obj->next;

	return new_obj;
}


/* ----- Graphics operations ----------------------------------------------- */


void record_line(void *ctx, int sx, int sy, int ex, int ey,
    int color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj =
	    new_obj(rec, ro_line, color, COLOR_NONE, layer);

	bb(&rec->bbox, sx, sy);
	bb(&rec->bbox, ex, ey);

	obj->x = sx;
	obj->y = sy;
	obj->u.line.ex = ex;
	obj->u.line.ey = ey;
}


void record_rect(void *ctx, int sx, int sy, int ex, int ey,
    int color, int fill_color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj =
	    new_obj(rec, ro_rect, color, fill_color, layer);

	bb(&rec->bbox, sx, sy);
	bb(&rec->bbox, ex, ey);

	obj->x = sx;
	obj->y = sy;
	obj->u.rect.ex = ex;
	obj->u.rect.ey = ey;
}


void record_poly(void *ctx,
    int points, const int x[points], const int y[points],
    int color, int fill_color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj =
	    new_obj(ctx, ro_poly, color, fill_color, layer);
	int i;
	unsigned  size;

	for (i = 0; i != points; i++)
		bb(&rec->bbox, x[i], y[i]);

	obj->u.poly.n = points;
	size = sizeof(int) * points;
	obj->u.poly.vx = alloc_size(size);
	obj->u.poly.vy = alloc_size(size);
	memcpy(obj->u.poly.vx, x, size);
	memcpy(obj->u.poly.vy, y, size);
}


void record_circ(void *ctx, int x, int y, int r,
    int color, int fill_color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj =
	    new_obj(ctx, ro_circ, color, fill_color, layer);

	bb(&rec->bbox, x - r, y - r);
	bb(&rec->bbox, x + r, y + r);

	obj->x = x;
	obj->y = y;
	obj->u.circ.r = r;
}


void record_arc(void *ctx, int x, int y, int r, int sa, int ea,
    int color, int fill_color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj = new_obj(ctx, ro_arc, color, fill_color, layer);

	bb(&rec->bbox, x - r, y - r);
	bb(&rec->bbox, x + r, y + r);

	obj->x = x;
	obj->y = y;
	obj->u.arc.r = r;
	obj->u.arc.sa = sa;
	obj->u.arc.ea = ea;
}


void record_text(void *ctx, int x, int y, const char *s, unsigned size,
    enum text_align align, int rot, enum text_style style,
    unsigned color, unsigned layer)
{
	struct record *rec = ctx;
	struct record_obj *obj =
	    new_obj(ctx, ro_text, color, COLOR_NONE, layer);
	struct record_bbox bbox;
	int width = rec->ops->text_width(rec->user, s, size, style);

	bb_init(&bbox);
	switch (align) {
	case text_min:
		bb_rot(&bbox, x, y, 0, -size, rot);
		bb_rot(&bbox, x, y, width, 0, rot);
		break;
	case text_mid:
		bb_rot(&bbox, x, y, -(width + 1) / 2, -size, rot);
		bb_rot(&bbox, x, y, (width + 1) / 2, 0, rot);
		break;
	case text_max:
		bb_rot(&bbox, x, y, -width, -size, rot);
		bb_rot(&bbox, x, y, 0, 0, rot);
		break;
	default:
		BUG("invalid alignment %d", align);
	}

	obj->u.text.bbox = bbox;
	bb(&rec->bbox, bbox.xmin, bbox.ymin);
	bb(&rec->bbox, bbox.xmax, bbox.ymax);

	obj->x = x;
	obj->y = y;
	obj->u.text.s = stralloc(s);
	obj->u.text.size = size;
	obj->u.text.align = align;
	obj->u.text.rot = rot;
	obj->u.text.style = style;
}


void record_set_extra(void *ctx, enum gfx_extra extra)
{
	struct record *rec = ctx;

	rec->extra = extra;
}


/* ----- Initialization and cleanup ---------------------------------------- */


void record_init(struct record *rec, const struct gfx_ops *ops, void *user)
{
	rec->ops = ops;
	rec->user = user;
	rec->extra = 0;
	bb_init(&rec->bbox);
	rec->layers = NULL;
}


/* ----- Reset for a new page ---------------------------------------------- */


/*
 * This is used to signal a new page. The caller kepps a copy of the entire
 * "struct record". The items on rec->layers are therefore not lost.
 */

void record_wipe(struct record *rec)
{
	rec->layers = NULL;
}


/* ----- Replay ------------------------------------------------------------ */


void record_replay(const struct record *rec, enum gfx_extra extra)
{
	const struct gfx_ops *ops = rec->ops;
	void *ctx = rec->user;
	const struct record_layer *layer;
	const struct record_obj *obj;

	for (layer = rec->layers; layer; layer = layer->next)
		for (obj = layer->objs; obj; obj = obj->next) {
			if (obj->extra && !(obj->extra & extra))
				continue;
			switch (obj->type) {
			case ro_line:
				ops->line(ctx, obj->x, obj->y,
				    obj->u.line.ex, obj->u.line.ey,
				    obj->color, layer->layer);
				break;
			case ro_rect:
				ops->rect(ctx, obj->x, obj->y,
				    obj->u.rect.ex, obj->u.rect.ey,
				    obj->color, obj->fill_color, layer->layer);
				break;
			case ro_poly:
				ops->poly(ctx, obj->u.poly.n,
				    obj->u.poly.vx, obj->u.poly.vy,
				    obj->color, obj->fill_color, layer->layer);
				break;
			case ro_circ:
				ops->circ(ctx, obj->x, obj->y,
				    obj->u.circ.r,
				    obj->color, obj->fill_color, layer->layer);
				break;
			case ro_arc:
				ops->arc(ctx, obj->x, obj->y, obj->u.arc.r,
				    obj->u.arc.sa, obj->u.arc.ea,
				    obj->color, obj->fill_color, layer->layer);
				break;
			case ro_text:
				ops->text(ctx, obj->x, obj->y, obj->u.text.s,
				    obj->u.text.size, obj->u.text.align,
				    obj->u.text.rot, obj->u.text.style,
				    obj->color, layer->layer);
				break;
			default:
				BUG("invalid object type %d", obj->type);
			}
		}
}


/* ----- Find text by position --------------------------------------------- */


const char *record_find_text_bbox(const struct record *rec,
    enum gfx_extra extra, int x, int y, struct record_bbox *bbox)
{
	const struct record_layer *layer;
	const struct record_obj *obj;

	for (layer = rec->layers; layer; layer = layer->next)
		for (obj = layer->objs; obj; obj = obj->next) {
			if (obj->extra && !(obj->extra & extra))
				continue;
			if (obj->type != ro_text)
				continue;
			*bbox = obj->u.text.bbox;
			if (x >= bbox->xmin && x <= bbox->xmax &&
			    y >= bbox->ymin && y <= bbox->ymax)
				return obj->u.text.s;
		}
	return NULL;
}


const char *record_find_text(const struct record *rec, enum gfx_extra extra,
    int x, int y)
{
	struct record_bbox bbox;

	return record_find_text_bbox(rec, extra, x, y, &bbox);
}


/* ----- Bounding box ------------------------------------------------------ */


void record_bbox(const struct record *rec, int *x, int *y, int *w, int *h)
{
	if (x)
		*x = rec->bbox.xmin;
	if (y)
		*y = rec->bbox.ymin;
	if (w)
		*w = rec->bbox.xmax - rec->bbox.xmin + 1;
	if (h)
		*h = rec->bbox.ymax - rec->bbox.ymin + 1;
}


/* ----- Cleanup ----------------------------------------------------------- */


static void record_obj_destroy(struct record_obj *obj)
{
	switch (obj->type) {
	case ro_poly:
		free(obj->u.poly.vx);
		free(obj->u.poly.vy);
		break;
	case ro_text:
		free(obj->u.text.s);
		break;
	default:
		break;
	}
	free(obj);
}


void record_destroy(struct record *rec)
{
	struct record_layer *next_layer;
	struct record_obj *next_obj;

	while (rec->layers) {
		next_layer = rec->layers->next;
		while (rec->layers->objs) {
			next_obj = rec->layers->objs->next;
			record_obj_destroy(rec->layers->objs);
			rec->layers->objs = next_obj;
		}
		free(rec->layers);
		rec->layers = next_layer;
	}
}
