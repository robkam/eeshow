/*
 * gfx/gfx.c - Generate graphical output for Eeschema items
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
#include <unistd.h>	/* for optind */

#include "misc/util.h"
#include "gfx/style.h"
#include "gfx/text.h"
#include "gfx/gfx.h"


struct gfx {
	const struct gfx_ops *ops;
	void *user;
	enum gfx_extra extra;
};


/* ----- Wrappers for graphics primitives ---------------------------------- */


void gfx_line(struct gfx *gfx,
    int sx, int sy, int ex, int ey, int color, unsigned layer)
{
	if (gfx->ops->line) {
		gfx->ops->line(gfx->user, sx, sy, ex, ey, color, layer);
		return;
	}

	int vx[] = { sx, ex };
	int vy[] = { sy, ey };

	gfx_poly(gfx, 2, vx, vy, color, COLOR_NONE, layer);
}


void gfx_rect(struct gfx *gfx, int sx, int sy, int ex, int ey,
    int color, int fill_color, unsigned layer)
{
	if (gfx->ops->rect) {
		gfx->ops->rect(gfx->user, sx, sy, ex, ey,
		    color, fill_color, layer);
		return;
	}

	int vx[] = { sx, ex, ex, sx, sx };
	int vy[] = { sy, sy, ey, ey, sy };

	gfx_poly(gfx, 5, vx, vy, color, fill_color, layer);
}


void gfx_poly(struct gfx *gfx,
    int points, const int x[points], const int y[points],
    int color, int fill_color, unsigned layer)
{
	gfx->ops->poly(gfx->user, points, x, y, color, fill_color, layer);
}


void gfx_circ(struct gfx *gfx,
    int x, int y, int r, int color, int fill_color, unsigned layer)
{
	gfx->ops->circ(gfx->user, x, y, r, color, fill_color, layer);
}


void gfx_arc(struct gfx *gfx, int x, int y, int r, int sa, int ea,
    int color, int fill_color, unsigned layer)
{
	gfx->ops->arc(gfx->user, x, y, r, sa, ea, color, fill_color, layer);
}


void gfx_text(struct gfx *gfx, int x, int y, const char *s, unsigned size,
    enum text_align align, int rot, enum text_style style,
    unsigned color, unsigned layer)
{
	gfx->ops->text(gfx->user, x, y, s, size, align, rot, style,
	    color, layer);
}


void gfx_tag(struct gfx *gfx, const char *s,
    unsigned points, const int x[points], int const y[points])
{
	if (gfx->ops->tag)
		gfx->ops->tag(gfx->user, s, points, x, y);
}


unsigned gfx_text_width(struct gfx *gfx, const char *s, unsigned size,
    enum text_style style)
{
	return gfx->ops->text_width(gfx->user, s, size, style);
}


enum gfx_extra gfx_set_extra(struct gfx *gfx, enum gfx_extra extra)
{
	enum gfx_extra old = gfx->extra;

	if (gfx->ops->set_extra)
		gfx->ops->set_extra(gfx->user, extra);
	gfx->extra = extra;
	return old;
}


/* ----- Initialization ---------------------------------------------------- */


struct gfx *gfx_init(const struct gfx_ops *ops)
{
	struct gfx *new;

	new = alloc_type(struct gfx);
	new->user = ops->init();
	new->ops = ops;
	new->extra = 0;
	return new;
}


bool gfx_args(struct gfx *gfx, int argc, char *const *argv, const char *opts)
{
	char *combined = (char *) opts;
	bool res;

	if (gfx->ops->opts)
		alloc_printf(&combined, "%s%s", opts, gfx->ops->opts);
	optind = 0;
	res = gfx->ops->args && gfx->ops->args(gfx->user, argc, argv, combined);
	if (gfx->ops->opts)
		free(combined);
	return res;
}


/* ----- Access ------------------------------------------------------------ */


void gfx_sheet_name(struct gfx *gfx, const char *name)
{
	if (gfx->ops->sheet_name)
		gfx->ops->sheet_name(gfx->user, name);
}


void gfx_new_sheet(struct gfx *gfx)
{
	if (gfx->ops->new_sheet)
		gfx->ops->new_sheet(gfx->user);
}


bool gfx_multi_sheet(struct gfx *gfx)
{
	return !!gfx->ops->new_sheet;
}


void *gfx_user(struct gfx *gfx)
{
	return gfx->user;
}


/* ----- Termination ------------------------------------------------------- */


void gfx_destroy(struct gfx *gfx)
{
	free(gfx);
}


int gfx_end(struct gfx *gfx, enum gfx_extra extra)
{
	int res = 0;

	if (gfx->ops->end)
		res = gfx->ops->end(gfx->user, extra);
	gfx_destroy(gfx);
	return res;
}
