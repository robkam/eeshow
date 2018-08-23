/*
 * kicad/dwg.c - Complex drawing functions for KiCad items
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/misc.h"
#include "gfx/style.h"
#include "gfx/text.h"
#include "gfx/gfx.h"
#include "kicad/dwg.h"


/* ----- Helper functions -------------------------------------------------- */


static void bbox_from_poly(struct dwg_bbox *bbox, unsigned n,
    const int *vx, const int *vy)
{
	unsigned i;
	int xmax, ymax;

	bbox->x = xmax = *vx;
	bbox->y = ymax = *vy;
	for (i = 1; i != n; i++) {
		if (vx[i] < bbox->x)
			bbox->x = vx[i];
		if (vy[i] < bbox->y)
			bbox->y = vy[i];
		if (vx[i] > xmax)
			xmax = vx[i];
		if (vy[i] > ymax)
			ymax = vy[i];
	}
	bbox->w = xmax - bbox->x + 1;
	bbox->h = ymax - bbox->y + 1;
}


/* ----- Labels ------------------------------------------------------------ */


enum box_type {		//  ___
	box_simple,	// [___]
	box_left,	// <___]
	box_right,	// [___>
	box_both,	// <___>
};


static enum box_type flip_box(enum box_type box)
{
	switch (box) {
	case box_simple:
		return box_simple;
	case box_left:
		return box_right;
	case box_right:
		return box_left;
	case box_both:
		return box_both;
	default:
		BUG("invalid box type %d", box);
	}
}


void dwg_label(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox)
{
	struct text txt = {
		.s	= s,
		.size	= dim,
		.x	= x,
		.y	= y,
		.rot	= 0,
		.hor	= 0,
		.vert	= text_min,
		.style	= style,
	};
	int dx = 0, dy = 0;

	switch (dir) {
	case 0:	/* right */
		txt.rot = 0;
		txt.hor = text_min;
		dy = 1;
		break;
	case 1:	/* up */
		txt.rot = 90;
		txt.hor = text_min;
		dx = -1;
		break;
	case 2:	/* left */
		txt.rot = 0;
		txt.hor = text_max;
		dy = 1;
		break;
	case 3:	/* down */
		txt.rot = 90;
		txt.hor = text_max;
		dx = -1;
		break;
	default:
		assert(0);
	}

	txt.y -= dy * LABEL_OFFSET;
	txt.x += dx * LABEL_OFFSET;
	text_show(&txt, gfx, COLOR_LABEL, LAYER_LABEL);
}


void dwg_glabel(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox)
{
	struct text txt = {
		.s	= s,
		.size	= dim,
		.x	= x,
		.y	= y,
		.rot	= 0,
		.hor	= 0,
		.vert	= text_mid,
		.style	= style,
	};
	int n = 6;
	int vx[7];
	int vy[7];
	int half = (dim >> 1) + GLABEL_OFFSET;
	enum box_type box;
	int dx, shift_flat, shift_tip;
	bool anchor_right = 1;
	char *tag;

	switch (shape) {
	case dwg_unspec:
		box = box_simple;
		break;
	case dwg_in:
		box = box_right;
		break;
	case dwg_out:
		box = box_left;
		break;
	case dwg_bidir:
		box = box_both;
		break;
	default:
		assert(0);
	}

	switch (dir) {
	case 0:	/* left */
		txt.rot = 0;
		txt.hor = text_max;
		dx = -1;
		break;
	case 1:	/* up */
		txt.rot = 90;
		txt.hor = text_min;
		dx = 1;
		box = flip_box(box);
		anchor_right = !anchor_right;
		break;
	case 2:	/* right */
		txt.rot = 0;
		txt.hor = text_min;
		dx = 1;
		box = flip_box(box);
		anchor_right = !anchor_right;
		break;
	case 3:	/* down */
		txt.rot = 90;
		txt.hor = text_max;
		dx = -1;
		break;
	default:
		assert(0);
	}

	shift_flat = dx * GLABEL_OFFSET;
	shift_tip = dx * (GLABEL_OFFSET + half);

	switch (box) {
	case box_simple:
		n = 5;
		text_shift(&txt, gfx, txt.hor, text_mid, shift_flat, 0);
		text_rel(&txt, gfx, text_min, text_min,
		    -GLABEL_OFFSET, GLABEL_OFFSET, vx + 1, vy + 1);
		text_rel(&txt, gfx, text_max, text_min,
		    GLABEL_OFFSET, GLABEL_OFFSET, vx + 2, vy + 2);
		text_rel(&txt, gfx, text_max, text_max,
		    GLABEL_OFFSET, -GLABEL_OFFSET, vx + 3, vy + 3);
		text_rel(&txt, gfx, text_min, text_max,
		    -GLABEL_OFFSET, -GLABEL_OFFSET, vx + 4, vy + 4);
		break;
	case box_right:
		text_shift(&txt, gfx, txt.hor, text_mid,
		    anchor_right ? shift_tip : shift_flat, 0);
		text_rel(&txt, gfx, text_min, text_min,
		    -GLABEL_OFFSET, GLABEL_OFFSET, vx + 1, vy + 1);
		text_rel(&txt, gfx, text_max, text_min,
		    GLABEL_OFFSET, GLABEL_OFFSET, vx + 2, vy + 2);
		text_rel(&txt, gfx, text_max, text_mid, GLABEL_OFFSET + half, 0,
		    vx + 3, vy + 3);
		text_rel(&txt, gfx, text_max, text_max,
		    GLABEL_OFFSET, -GLABEL_OFFSET, vx + 4, vy + 4);
		text_rel(&txt, gfx, text_min, text_max,
		    -GLABEL_OFFSET, -GLABEL_OFFSET, vx + 5, vy + 5);
		break;
	case box_left:
		text_shift(&txt, gfx, txt.hor, text_mid,
		    anchor_right ? shift_flat : shift_tip, 0);
		text_rel(&txt, gfx, text_min, text_min,
		    -GLABEL_OFFSET, GLABEL_OFFSET, vx + 1, vy + 1);
		text_rel(&txt, gfx, text_max, text_min,
		    GLABEL_OFFSET, GLABEL_OFFSET, vx + 2, vy + 2);
		text_rel(&txt, gfx, text_max, text_max,
		    GLABEL_OFFSET, -GLABEL_OFFSET, vx + 3, vy + 3);
		text_rel(&txt, gfx, text_min, text_max,
		    -GLABEL_OFFSET, -GLABEL_OFFSET, vx + 4, vy + 4);
		text_rel(&txt, gfx, text_min, text_mid, -GLABEL_OFFSET- half, 0,
		    vx + 5, vy + 5);
		break;
	case box_both:
		n = 7;
		text_shift(&txt, gfx, txt.hor, text_mid, shift_tip, 0);
		text_rel(&txt, gfx, text_min, text_min,
		    -GLABEL_OFFSET, GLABEL_OFFSET, vx + 1, vy + 1);
		text_rel(&txt, gfx, text_max, text_min,
		    GLABEL_OFFSET, GLABEL_OFFSET, vx + 2, vy + 2);
		text_rel(&txt, gfx, text_max, text_mid, GLABEL_OFFSET + half, 0,
		    vx + 3, vy + 3);
		text_rel(&txt, gfx, text_max, text_max,
		    GLABEL_OFFSET, -GLABEL_OFFSET, vx + 4, vy + 4);
		text_rel(&txt, gfx, text_min, text_max,
		    -GLABEL_OFFSET, -GLABEL_OFFSET, vx + 5, vy + 5);
		text_rel(&txt, gfx, text_min, text_mid, -GLABEL_OFFSET- half, 0,
		    vx + 6, vy + 6);
		break;
	default:
		assert(0);
	}

	text_show(&txt, gfx, COLOR_GLABEL, LAYER_GLABEL);

	vx[0] = vx[n - 1];
	vy[0] = vy[n - 1];
	gfx_poly(gfx, n, vx, vy, COLOR_GLABEL, COLOR_NONE, LAYER_GLABEL);

	if (bbox)
		bbox_from_poly(bbox, n, vx, vy);

	alloc_printf(&tag, "G:%s", s);
	gfx_tag(gfx, tag, n, vx, vy);
	free(tag);
}


static int make_box(enum box_type box, int h, int *vx, int *vy)
{
	int r = h / 2;

	switch (box) {
	case box_simple:
		vx[0] = 0;
		vy[0] = -r;
		vx[1] = 2 * r;
		vy[1] = -r;
		vx[2] = 2 * r;
		vy[2] = r;
		vx[3] = 0;
		vy[3] = r;
		return 4;
	case box_right:
		vx[0] = 0;
		vy[0] = -r;
		vx[1] = r;
		vy[1] = -r;
		vx[2] = 2 * r;
		vy[2] = 0;
		vx[3] =  r;
		vy[3] = r;
		vx[4] = 0;
		vy[4] = r;
		return 5;
	case box_left:
		vx[0] = r;
		vy[0] = -r;
		vx[1] = 2 * r;
		vy[1] = -r;
		vx[2] = 2 * r;
		vy[2] = r;
		vx[3] = r;
		vy[3] = r;
		vx[4] = 0;
		vy[4] = 0;
		return 5;
	case box_both:
		vx[0] = 0;
		vy[0] = 0;
		vx[1] = r;
		vy[1] = -r;
		vx[2] = 2 * r;
		vy[2] = 0;
		vx[3] = r;
		vy[3] = r;
		return 4;
	default:
		assert(0);
	}
}


void dwg_hlabel(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox)
{
	struct text txt = {
		.s	= s,
		.size	= dim,
		.x	= x,
		.y	= y,
		.rot	= 0,
		.hor	= 0,
		.vert	= text_mid,
		.style	= style,
	};
	int vx[6], vy[6];
	int rot;
	int n, i;

	switch (shape) {
	case dwg_unspec:
		n = make_box(box_simple, dim, vx, vy);
		break;
	case dwg_in:
		n = make_box(box_left, dim, vx, vy);
		break;
	case dwg_out:
		n = make_box(box_right, dim, vx, vy);
		break;
	case dwg_tri:
	case dwg_bidir:
		n = make_box(box_both, dim, vx, vy);
		break;
	default:
		assert(0);
	}

	switch (dir) {
	case 0:	/* right */
		rot = 180;
		txt.hor = text_max;
		break;
	case 1:	/* up */
		rot = 90;
		txt.hor = text_min;
		break;
	case 2:	/* left */
		rot = 0;
		txt.hor = text_min;
		break;
	case 3:	/* down */
		rot = 270;
		txt.hor = text_max;
		break;
	default:
		assert(0);
	}

	txt.x += rx((1 + HLABEL_OFFSET_F) * dim, 0, rot);
	txt.y += ry((1 + HLABEL_OFFSET_F)  * dim, 0, rot);

	for (i = 0; i != n; i++) {
		int tmp;

		tmp = x + rx(vx[i], vy[i], rot);
		vy[i] = y + ry(vx[i], vy[i], rot);
		vx[i] = tmp;
	}

	vx[n] = vx[0];
	vy[n] = vy[0];

	txt.rot = rot % 180;

	text_show(&txt, gfx, COLOR_HLABEL, LAYER_HLABEL);
	gfx_poly(gfx, n + 1, vx, vy, COLOR_HLABEL, COLOR_NONE, LAYER_HLABEL);
}


/* ----- Text -------------------------------------------------------------- */


void dwg_text(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox)
{
	struct text txt = {
		.s	= s,
		.size	= dim,
		.x	= x,
		.y	= y,
		.rot	= 0,
		.hor	= text_min,
		.vert	= text_min,
		.style	= style,
	};

	switch (dir) {
	case 0:	/* right */
		break;
	case 1:	/* up */
		text_rot(&txt, 90);
		break;
	case 2:	/* left */
		txt.hor = text_max;
		break;
	case 3:	/* down */
		text_rot(&txt, 90);
		txt.hor = text_max;
		break;
	default:
		assert(2 + 2 == 5);
	}

	text_show(&txt, gfx, COLOR_TEXT, LAYER_TEXT);
}


/* ----- Connections ------------------------------------------------------- */


void dwg_junction(struct gfx *gfx, int x, int y)
{
	gfx_circ(gfx, x, y, JUNCTION_R, COLOR_NONE, COLOR_WIRE, LAYER_WIRES);
}


void dwg_noconn(struct gfx *gfx, int x, int y)
{
	int vx[2] = { x - NOCONN_LEN, x + NOCONN_LEN };
	int vy[2] = { y - NOCONN_LEN, y + NOCONN_LEN };

	gfx_poly(gfx, 2, vx, vy, COLOR_NOCONN, COLOR_NONE, LAYER_NOCONN);
	swap(vy[0], vy[1]);
	gfx_poly(gfx, 2, vx, vy, COLOR_NOCONN, COLOR_NONE, LAYER_NOCONN);
}


/* ----- Lines ------------------------------------------------------------- */

/*
 * We can't use gfx_poly because lines are dashed and we don't have that
 * property at the gfx_poly API.
 *
 * Since dashing may produce different results between going from A to B and
 * going from B to A, we enforce a common direction, so that pixel diffs will
 * treat reversed lines as still equal.
 */

void dwg_line(struct gfx *gfx, int sx, int sy, int ex, int ey)
{
	if (sx < ex || (sx == ex && sy < ey))
		gfx_line(gfx, sx, sy, ex, ey, COLOR_SHEET_DWG, LAYER_LINES);
	else
		gfx_line(gfx, ex, ey, sx, sy, COLOR_SHEET_DWG, LAYER_LINES);
}


/* ----- Wires and busses -------------------------------------------------- */


void dwg_wire(struct gfx *gfx, int sx, int sy, int ex, int ey)
{
	int vx[] = { sx, ex };
	int vy[] = { sy, ey };

	// WIDTH_WIRE
	gfx_poly(gfx, 2, vx, vy, COLOR_WIRE, COLOR_NONE, LAYER_WIRES);
}


void dwg_bus(struct gfx *gfx, int sx, int sy, int ex, int ey)
{
	int vx[] = { sx, ex };
	int vy[] = { sy, ey };

	// WIDTH_BUS
	gfx_poly(gfx, 2, vx, vy, COLOR_BUS, COLOR_NONE, LAYER_BUSSES);
}
