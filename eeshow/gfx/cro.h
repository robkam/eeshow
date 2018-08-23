/*
 * gfx/cro.h - Cairo graphics back-end
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef GFX_CRO_H
#define	GFX_CRO_H

#include <stdbool.h>
#include <stdint.h>

#include <cairo/cairo.h>

#include "gfx/gfx.h"


struct cro_ctx;


extern bool use_pango;
extern bool disable_overline;

extern const struct gfx_ops cro_png_ops;
extern const struct gfx_ops cro_pdf_ops;
extern const struct gfx_ops cro_ps_ops;
extern const struct gfx_ops cro_eps_ops;
extern const struct gfx_ops cro_svg_ops;

#define	cro_img_ops	cro_png_ops	/* just don't call cro_img_ops.end */
#define	cro_canvas_ops	cro_png_ops	/* just don't call cro_canvas_ops.end */


void cro_color_override(struct cro_ctx *cc, int color);

void cro_get_size(const struct cro_ctx *cc, int *w, int *h, int *x, int *y);

uint32_t *cro_img_end(struct cro_ctx *cc, int *w, int *h, int *stride,
    enum gfx_extra extra);
void cro_img_write(cairo_surface_t *s, const char *name);
void cro_img_reset(struct cro_ctx *cc);
void cro_img_destroy(struct cro_ctx *cc);

void cro_canvas_end(struct cro_ctx *cc, int *w, int *h, int *xmin, int *ymin);
void cro_canvas_prepare(cairo_t *cr);
void cro_canvas_draw(struct cro_ctx *cc, cairo_t *cr,
    int x, int y, float scale, enum gfx_extra extra);

uint32_t *cro_img(struct cro_ctx *cc, enum gfx_extra extra,
    int x0, int yo, int w, int h,
    float scale, double alpha, cairo_t **res_cr, int *res_stride);
cairo_surface_t *cro_img_surface(struct cro_ctx *cc);

#endif /* !GFX_CRO_H */
