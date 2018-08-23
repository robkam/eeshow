/*
 * gfx/text.c - FIG text object
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/misc.h"
#include "gfx/style.h"
#include "gfx/gfx.h"
#include "gfx/text.h"


void text_init(struct text *txt)
{
	txt->s = NULL;
	txt->size = 0;
	txt->x = txt->y = 0;
	txt->rot = 0;
	txt->hor = text_mid;
	txt->vert = text_mid;
}


void text_free(struct text *txt)
{
	free((void *) txt->s);
	txt->s = NULL;
}


void text_set(struct text *txt, const char *s)
{
	free((void *) txt->s);
	txt->s = stralloc(s);
}


void text_rot(struct text *txt, int deg)
{
	txt->rot = angle_add(txt->rot, deg);
}


enum text_align text_flip(enum text_align align)
{
	switch (align) {
	case text_min:
		return text_max;
	case text_mid:
		return text_mid;
	case text_max:
		return text_min;
	default:
		BUG("invalid alignment %d", align);
	}
}


void text_flip_x(struct text *txt)
{
	txt->rot = angle_add(txt->rot, 180);
	txt->hor = text_flip(txt->hor);
	// @@@ flip vert, too ?
}


static int align(int dim, enum text_align align)
{
	switch (align) {
	case text_min:
		return 0;
	case text_mid:
		return dim / 2;
	case text_max:
		return dim;
	default:
		BUG("invalid alignment %d", align);
	}
}


void text_show(const struct text *txt, struct gfx *gfx,
    int color, unsigned layer)
{
	char *buf = stralloc(txt->s);
	char *tmp = buf;
	const char *s;
	int x = txt->x;
	int y = txt->y;
	unsigned multiline = 0;

	for (s = txt->s; *s; s++)
		if (*s == '\n')
			multiline += NEWLINE_SKIP * txt->size;
	x += rx(0, align(txt->size + multiline, txt->vert) - multiline,
	    txt->rot);
	y += ry(0, align(txt->size + multiline, txt->vert) - multiline,
	    txt->rot);
	while (1) {
		s = strtok(tmp, "\n");
		if (!s)
			break;
		tmp = NULL;
		gfx_text(gfx, x, y, s, txt->size, txt->hor, txt->rot,
		    txt->style, color, layer);
		x += rx(0, NEWLINE_SKIP * txt->size, txt->rot);
		y += ry(0, NEWLINE_SKIP * txt->size, txt->rot);
	}
	free(buf);
}


void text_rel(const struct text *txt, struct gfx *gfx,
    enum text_align xr, enum text_align yr,
    int dx, int dy, int *res_x, int *res_y)
{
	int width = gfx_text_width(gfx, txt->s, txt->size, txt->style);

	dx -= align(width, txt->hor);
	dy += align(txt->size, txt->vert);
	dx += align(width, xr);
	dy -= align(txt->size, yr);
	if (res_x)
		*res_x = txt->x + rx(dx, dy, txt->rot);
	if (res_y)
		*res_y = txt->y + ry(dx, dy, txt->rot);
}


void text_shift(struct text *txt, struct gfx *gfx,
    enum text_align xr, enum text_align yr, int dx, int dy)
{
	text_rel(txt, gfx, xr, yr, dx, dy, &txt->x, &txt->y);
}


int text_rel_x(const struct text *txt, struct gfx *gfx,
    enum text_align xr, enum text_align yr, int dx, int dy)
{
	int x;

	text_rel(txt, gfx, xr, yr, dx, dy, &x, NULL);
	return x;
}


int text_rel_y(const struct text *txt, struct gfx *gfx,
    enum text_align xr, enum text_align yr, int dx, int dy)
{
	int y;

	text_rel(txt, gfx, xr, yr, dx, dy, NULL, &y);
	return y;
}
