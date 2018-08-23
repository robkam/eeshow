/*
 * gui/over.c - GUI: overlays
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Resources:
 *
 * http://zetcode.com/gfx/cairo/cairobackends/
 * https://developer.gnome.org/gtk3/stable/gtk-migrating-2-to-3.html
 * https://www.cairographics.org/samples/rounded_rectangle/
 *
 * Section "Description" in
 * https://developer.gnome.org/pango/stable/pango-Cairo-Rendering.html
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <cairo/cairo.h>
#include <pango/pangocairo.h>

#include "misc/util.h"
#include "gui/fmt-pango.h"
#include "gui/aoi.h"
#include "gui/style.h"
#include "gui/over.h"


struct overlay {
	const char *s;
	cairo_surface_t *icon;

	struct overlay_style style;

	struct aoi **aois;
	bool (*hover)(void *user, bool on, int dx, int dy);
	void (*click)(void *user);
	void *user;

	struct aoi *aoi;
	const struct overlay *related;

	int x,y;		/* for over_iterate_geometry */
	unsigned w, h;

	struct overlay *next, *prev;
};


/* ----- Drawing helper functions ------------------------------------------ */


static void rrect(cairo_t *cr, double x, double y, double w, double h, int r)
{
	const double deg = M_PI / 180.0;

	cairo_new_path(cr);
	cairo_arc(cr, x + w - r, y + r, r, -90 * deg, 0);
	cairo_arc(cr, x + w - r, y + h - r, r, 0, 90 * deg);
	cairo_arc(cr, x + r, y + h - r, r, 90 * deg, 180 * deg);
	cairo_arc(cr, x + r, y + r, r, 180 * deg, 270 * deg);
	cairo_close_path(cr);
}


static void background(const struct overlay *over, cairo_t *cr,
    int x, int y, unsigned w, unsigned h)
{
	const struct overlay_style *style = &over->style;
	const struct color *bg = &style->bg;
	const struct color *frame = &style->frame;
	double center;

	center = style->width / 2.0;
	rrect(cr, x - center, y - center, w + style->width, h + style->width,
	    style->radius);

	cairo_set_source_rgba(cr, bg->r, bg->g, bg->b, bg->alpha);
	cairo_fill_preserve(cr);
	cairo_set_source_rgba(cr, frame->r, frame->g, frame->b, frame->alpha);
	cairo_set_line_width(cr, style->width);
	cairo_stroke(cr);
}


static void post_aoi(struct overlay *over, int x, int y, unsigned w, unsigned h)
{
	struct aoi aoi_cfg = {
		.x	= x,
		.y	= y,
		.w	= w,
		.h	= h,
		.hover	= over->hover,
		.click	= over->click,
		.user	= over->user,
	};

	over->x = x;
	over->y = y;
	over->w = w;
	over->h = h;

	if (!over->hover && !over->click)
		return;

	if (over->aoi) {
		aoi_update(over->aoi, &aoi_cfg);
	} else {
		over->aoi = aoi_add(over->aois, &aoi_cfg);
		if (over->related) {
			assert(over->related->aoi);
			aoi_set_related(over->aoi, over->related->aoi);
		}
	}
}


/* ----- Drawing text ------------------------------------------------------ */


static unsigned overlay_draw_text(struct overlay *over, cairo_t *cr,
    int x, int y, int dx, int dy)
{
	const struct overlay_style *style = &over->style;
	const struct color *fg = &style->fg;
	unsigned ink_w, ink_h;	/* effectively used text area size */
	unsigned w, h;		/* box size */
	int tx, ty;		/* text start position */

	PangoLayout *layout;
	PangoFontDescription *desc;
	PangoRectangle ink_rect;

	desc = pango_font_description_from_string(style->font);
	layout = pango_cairo_create_layout(cr);
	pango_layout_set_font_description(layout, desc);
	pango_layout_set_markup(layout, over->s, -1);
	pango_font_description_free(desc);

	pango_layout_get_extents(layout, &ink_rect, NULL);
#if 0
fprintf(stderr, "%d + %d  %d + %d\n",
    ink_rect.x / PANGO_SCALE, ink_rect.width / PANGO_SCALE,
    ink_rect.y / PANGO_SCALE, ink_rect.height / PANGO_SCALE);
#endif
	ink_w = ink_rect.width / PANGO_SCALE;
	ink_h = ink_rect.height / PANGO_SCALE;

	ink_w = ink_w > style->wmin ? ink_w : style->wmin;
	ink_w = !style->wmax || ink_w < style->wmax ? ink_w : style->wmax;
	w = ink_w + 2 * style->pad;

	ink_h = ink_h > style->hmin ? ink_h : style->hmin;
	ink_h = !style->hmax || ink_h < style->hmax ? ink_h : style->hmax;
	h = ink_h + 2 * style->pad;

	if (dx < 0)
		x -= w;
	if (dy < 0)
		y -= h;

	tx = x - ink_rect.x / PANGO_SCALE + style->pad;
	ty = y - ink_rect.y / PANGO_SCALE + style->pad;

	background(over, cr, x, y, w, h);

	if (style->wmax) {
		cairo_new_path(cr);
#if 0
fprintf(stderr, "%u(%d) %u %.60s\n", ty, ink_rect.y / PANGO_SCALE, ink_h, over->s);
#endif
/*
 * @@@ for some mysterious reason, we get
 * ink_h = ink_rect.height / PANGO_SCALE = 5
 * instead of 2 if using overlay_style_dense_selected. Strangely, changing
 * overlay_style_dense_selected such that it becomes more like
 * overlay_style_dense has no effect.
 *
 * This causes the text to be cut vertically, roughly in the middle. We hack
 * around this problem by growind the clipping area vertically. This works,
 * since we're currently only concerned about horizontal clipping anyway.
 */

		cairo_rectangle(cr, tx, ty, ink_w + 1, ink_h + 20);
		cairo_clip(cr);
	}

	cairo_set_source_rgba(cr, fg->r, fg->g, fg->b, fg->alpha);
	cairo_move_to(cr, tx, ty);

	pango_cairo_update_layout(cr, layout);
	pango_cairo_show_layout(cr, layout);
	cairo_reset_clip(cr);
	g_object_unref(layout);

	post_aoi(over, x, y, w, h);

	return h;
}


/* ----- Drawing an icon --------------------------------------------------- */


static unsigned overlay_draw_icon(struct overlay *over, cairo_t *cr,
    int x, int y, int dx, int dy)
{
	const struct overlay_style *style = &over->style;
	unsigned iw, ih;	/* icon size */
	unsigned w, h;		/* box size */
	int ix, iy;		/* icon start position */

	iw = cairo_image_surface_get_width(over->icon);
	iw = iw > style->wmin ? iw : style->wmin;
	iw = !style->wmax || iw < style->wmax ? iw : style->wmax;

	ih = cairo_image_surface_get_height(over->icon);
	ih = ih > style->hmin ? ih : style->hmin;
	ih = !style->hmax || ih < style->hmax ? ih : style->hmax;

	w = iw + 2 * style->pad;
	h = ih + 2 * style->pad;

	if (dx < 0)
		x -= w;
	if (dy < 0)
		y -= h;

	ix = x + style->pad;
	iy = y + style->pad;

	background(over, cr, x, y, w, h);

	cairo_set_source_surface(cr, over->icon, ix, iy);
	cairo_paint(cr);

	post_aoi(over, x, y, w, h);

	return h;
}


/* ----- Drawing interfaces ------------------------------------------------ */


unsigned overlay_draw(struct overlay *over, cairo_t *cr,
    int x, int y, int dx, int dy)
{
	const struct overlay_style *style = &over->style;
	unsigned w, h;

	if (over->s)
		return overlay_draw_text(over, cr, x, y, dx, dy);
	if (over->icon)
		return overlay_draw_icon(over, cr, x, y, dx, dy);

	w = style->wmin + 2 * style->pad;
	h = style->hmin + 2 * style->pad;
	background(over, cr, x, y, w, h);
	post_aoi(over, x, y, w, h);
	return h;
}


void overlay_draw_all_d(struct overlay *overlays, cairo_t *cr,
    int x, int y, int dx, int dy)
{
	struct overlay *over = overlays;
	unsigned h;

	if (dy < 0)
		while (over && over->next)
			over = over->next;
	while (over) {
		h = overlay_draw(over, cr, x, y, dx, dy);
		y += dy * (h + over->style.skip);
		if (dy >= 0)
			over = over->next;
		else
			over = over->prev;
	}
}


void overlay_draw_all(struct overlay *overlays, cairo_t *cr, int x, int y)
{
	int dx = 1;
	int dy = 1;

	if (x < 0 || y < 0) {
		double x1, y1, x2, y2;
		int sw, sh;

		cairo_clip_extents(cr, &x1, &y1, &x2, &y2);
		sw = x2 - x1;
		sh = y2 - y1;
		if (x < 0) {
			x = sw + x;
			dx = -1;
		}
		if (y < 0) {
			y = sh + y;
			dy = -1;
		}
	}

	overlay_draw_all_d(overlays, cr, x, y, dx, dy);
}


/* ----- Sizing text ------------------------------------------------------- */


static void overlay_size_text(const struct overlay *over,
    PangoContext *pango_context, int *w, int *h)
{
	const struct overlay_style *style = &over->style;
	PangoLayout *layout;
	PangoFontDescription *desc;
	PangoRectangle ink_rect;
	unsigned ink_w, ink_h;	/* effectively used text area size */

	/*
	 * Note that we need the caller to provide the Cairo context, because
	 * the font size changes subtly even between image (which we could
	 * create locally) and screen (which is better left to the outside
	 * world).
	 */

	desc = pango_font_description_from_string(style->font);
	layout = pango_layout_new(pango_context);
	pango_layout_set_font_description(layout, desc);
	pango_layout_set_markup(layout, over->s, -1);
	pango_font_description_free(desc);

	pango_layout_get_extents(layout, &ink_rect, NULL);
	g_object_unref(layout);

	ink_w = ink_rect.width / PANGO_SCALE;
	ink_h = ink_rect.height / PANGO_SCALE;

	ink_w = ink_w > style->wmin ? ink_w : style->wmin;
	ink_w = !style->wmax || ink_w < style->wmax ? ink_w : style->wmax;

	ink_h = ink_h > style->hmin ? ink_h : style->hmin;
	ink_h = !style->hmax || ink_h < style->hmax ? ink_h : style->hmax;

	if (w)
		*w = ink_w + 2 * style->pad;
	if (h)
		*h = ink_h + 2 * style->pad;
}


/* ----- Sizing icons ------------------------------------------------------ */


static void overlay_size_icon(const struct overlay *over, int *w, int *h)
{
	const struct overlay_style *style = &over->style;
	unsigned iw, ih;	/* effectively used icon size */

	iw = cairo_image_surface_get_width(over->icon);
	iw = iw > style->wmin ? iw : style->wmin;
	iw = !style->wmax || iw < style->wmax ? iw : style->wmax;

	ih = cairo_image_surface_get_height(over->icon);
	ih = ih > style->hmin ? ih : style->hmin;
	ih = !style->hmax || ih < style->hmax ? ih : style->hmax;

	if (w)
		*w = iw + 2 * style->pad;
	if (h)
		*h = ih + 2 * style->pad;
}


/* ----- Sizing ------------------------------------------------------------ */


void overlay_size(const struct overlay *over, PangoContext *pango_context,
    int *w, int *h)
{
	const struct overlay_style *style = &over->style;

	if (over->s)
		overlay_size_text(over, pango_context, w, h);
	else if (over->icon)
		overlay_size_icon(over, w, h);
	else {
		if (w)
			*w = style->wmin + 2 * style->pad;
		if (h)
			*h = style->hmin + 2 * style->pad;
	}
}


void overlay_size_all(const struct overlay *overlays,
    PangoContext *pango_context, bool dx, bool dy, int *w, int *h)
{
	const struct overlay *over;
	int w1, h1;

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	for (over = overlays; over; over = over->next) {
		int skip = over == overlays ? 0 : over->style.skip;

		overlay_size(over, pango_context, &w1, &h1);
		if (w) {
			if (dx)
				*w += w1 + skip;
			else
				*w = *w > w1 ? *w : w1;
		}
		if (h) {
			if (dy)
				*h += h1 + skip;
			else
				*h = *h > h1 ? *h : h1;
		}
	}
}


/* ----- Creation ---------------------------------------------------------- */


struct overlay *overlay_add(struct overlay **overlays, struct aoi **aois,
    bool (*hover)(void *user, bool on, int dx, int dy),
    void (*click)(void *user), void *user)
{
	struct overlay *over, *prev;
	struct overlay **anchor;

	over = alloc_type(struct overlay);
	over->s = NULL;
	over->icon = NULL;
	over->style = overlay_style_default;

	over->aois = aois;
	over->hover = hover;
	over->click = click;
	over->user = user;
	over->aoi = NULL;
	over->related = NULL;

	prev = NULL;
	for (anchor = overlays; *anchor; anchor = &(*anchor)->next)
		prev = *anchor;
	over->next = NULL;
	over->prev = prev;
	*anchor = over;

	return over;
}


/* ----- Configuration ----------------------------------------------------- */


void overlay_style(struct overlay *over, const struct overlay_style *style)
{
	over->style = *style;
}


void overlay_text_raw(struct overlay *over, const char *s)
{
	free((char *) over->s);
	over->s = stralloc(s);
}


void overlay_vtext(struct overlay *over, const char *fmt, va_list ap)
{
	overlay_text_raw(over, vfmt_pango(fmt, ap));
}


void overlay_text(struct overlay *over, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	overlay_vtext(over, fmt, ap);
	va_end(ap);
}


void overlay_icon(struct overlay *over, cairo_surface_t *icon)
{
	assert(!over->s);
	over->icon = cairo_surface_reference(icon);
}


/* ----- Nesting ----------------------------------------------------------- */


void overlay_set_related(struct overlay *over, struct overlay *related)
{
	/*
	 * Relatedness is a property that only matters to AoIs, but we have to
	 * defer propagating it because we only know our AoI after drawing the
	 * overlay.
	 */

	assert(!over->related);
	over->related = related;
}


void overlay_set_related_all(struct overlay *overlays, struct overlay *related)
{
	struct overlay *over;

	for (over = overlays; over; over = over->next)
		overlay_set_related(over, related);
}


/* ----- Iterate for geometry ---------------------------------------------- */


void over_iterate_geometry(const struct overlay *over,
    void (*fn)(void *user, void *user_over, int x, int y, unsigned w,
    unsigned h, int dy), void *user)
{
	while (over) {
		fn(user, over->user, over->x, over->y, over->w, over->h,
		    over->next ? over->next->y - over->y : 0);
		over = over->next;
	}
}


/* ----- Removal ----------------------------------------------------------- */


static void overlay_free(struct overlay *over)
{
	if (over->aoi)
		aoi_remove(over->aois, over->aoi);
	free((void *) over->s);
	if (!over->s)
		cairo_surface_destroy(over->icon);
	free(over);
}


void overlay_remove(struct overlay **overlays, struct overlay *over)
{
	if (over->next)
		over->next->prev = over->prev;
	if (over->prev)
		over->prev->next = over->next;
	else
		*overlays = over->next;
	overlay_free(over);
}


void overlay_remove_all(struct overlay **overlays)
{
	struct overlay *next;

	while (*overlays) {
		next = (*overlays)->next;
		overlay_remove(overlays, *overlays);
		*overlays = next;
	}
}
