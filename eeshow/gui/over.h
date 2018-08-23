/*
 * gui/over.h - GUI: overlays
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_OVER_H
#define	GUI_OVER_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include <cairo/cairo.h>
#include <pango/pangocairo.h>


#include "gui/aoi.h"


struct color {
	double r, g, b, alpha;
};


struct overlay_style {
	const char *font;
	unsigned wmin, wmax;
	unsigned hmin, hmax;
	unsigned radius;
	unsigned pad;	/* in x and y direction; adjust for radius ! */
	unsigned skip;	/* should be list-specific */
	struct color fg;
	struct color bg;
	struct color frame;
	double width;
};

struct overlay;


unsigned overlay_draw(struct overlay *over, cairo_t *cr,
    int x, int y, int dx, int dy);
void overlay_draw_all_d(struct overlay *overlays, cairo_t *cr,
    int x, int y, int dx, int dy);
void overlay_draw_all(struct overlay *overlays, cairo_t *cr, int x, int y);

void overlay_size(const struct overlay *over, PangoContext *pango_context,
    int *w, int *h);
void overlay_size_all(const struct overlay *overlays,
    PangoContext *pango_context, bool dx, bool dy, int *w, int *h);

struct overlay *overlay_add(struct overlay **overlays, struct aoi **aois,
    bool (*hover)(void *user, bool on, int dx, int dy),
    void (*click)(void *user), void *user);

void overlay_text_raw(struct overlay *over, const char *s);
void overlay_vtext(struct overlay *over, const char *fmt, va_list ap);
void overlay_text(struct overlay *over, const char *fmt, ...);
void overlay_icon(struct overlay *over, cairo_surface_t *icon);
void overlay_style(struct overlay *over, const struct overlay_style *style);

void overlay_set_related(struct overlay *over, struct overlay *related);
void overlay_set_related_all(struct overlay *overlays, struct overlay *related);

void over_iterate_geometry(const struct overlay *over,
    void (*fn)(void *user, void *user_over, int x, int y, unsigned w,
    unsigned h, int dy), void *user);

void overlay_remove(struct overlay **overlays, struct overlay *over);
void overlay_remove_all(struct overlay **overlays);

#endif /* !GUI_OVER_H */
