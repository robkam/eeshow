/*
 * gfx/diff.h - Schematics difference
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef GFX_DIFF_H
#define	GFX_DIFF_H

#include <stdint.h>

#include <cairo/cairo.h>

#include "gfx/gfx.h"
#include "gfx/cro.h"
#include "kicad/ext.h"


extern const struct gfx_ops diff_ops;


struct area {
	int xa, ya, xb, yb;
	uint32_t color;
	struct area *next;
};

struct diff;


void add_area(struct area **areas, int xa, int ya, int xb, int yb,
    uint32_t color);
void free_areas(struct area **areas);

void diff_to_canvas(cairo_t *cr, int cx, int cy, float scale,
    struct cro_ctx *old, struct cro_ctx *new, enum gfx_extra extra,
    const struct area *areas);

void *diff_process_file(struct diff *diff, struct file_names *file_names,
    int argc, char *const *argv, const char *opts);

#endif /* !GFX_DIFF_H */
