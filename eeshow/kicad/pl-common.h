/*
 * kicad/pl-common.h - Shared definitions for KiCad page layout handling
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_PL_COMMON_H
#define	KICAD_PL_COMMON_H

#include "gfx/text.h"
#include "kicad/sexpr.h"


enum font_flag {
	font_bold	= 1 << 0,
	font_italic	= 1 << 1,
};

enum page_constraint {
	pc_none,
	pc_only_one,
	pc_except_one,
};


struct pl_point {
	float x, y;
	struct pl_point *next;
};

struct pl_poly {
	struct pl_point *points;
	struct pl_poly *next;
};

struct pl_obj {
	enum pl_obj_type {
		pl_obj_rect,
		pl_obj_line,
		pl_obj_text,
		pl_obj_poly,
	} type;

	enum page_constraint pc;
	const char *s;		/* tbtext */
	int repeat;		/* line, rect, tbtext */
	float x, y;		/* line, rect, tbtext, poly */
	int dx, dy;
	float ex, ey;		/* line, rect; font size for tbtext */
	int edx, edy;
	float incrx, incry;
	int incrlabel;		/* tbtext */
	int font;		/* tbtext (flags) */
	enum text_align hor;	/* tbtext */
	enum text_align vert;	/* tbtext */
	float rotate;		/* tbtext, poly */
	struct pl_poly *poly;	/* poly */

	struct pl_obj *next;
};

struct pl_ctx {
	struct sexpr_ctx *sexpr_ctx;
	float l, r, t, b;	/* margins */
	float tx, ty;		/* text size */
	struct pl_obj *objs;
};

#endif /* !KICAD_PL_COMMON_H */
