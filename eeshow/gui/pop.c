/*
 * gui/pop.c - Pop-up helper functions
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
#include <stdarg.h>

#include <gtk/gtk.h>

#include "gui/style.h"
#include "gui/over.h"
#include "gui/common.h"
#include "gui/pop.h"


/* small offset to hide rounding errors */
#define	CHEAT	1


void eeschema_coord(const struct gui *gui, int x, int y, int *rx, int *ry)
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(gui->da, &alloc);
	*rx = ((x - gui->x) * gui->scale) + alloc.width / 2;
	*ry = ((y - gui->y) * gui->scale) + alloc.height / 2;
}


/* ----- Hovering ---------------------------------------------------------- */


void dehover_pop(struct gui *gui)
{
	overlay_remove_all(&gui->pop_overlays);
	overlay_remove_all(&gui->pop_underlays);
	gui->pop_origin = NULL;
	redraw(gui);
}



static bool pop_hover(void *user, bool on, int dx, int dy)
{
	struct gui *gui = user;

	if (!on)
		dehover_pop(gui);
	return 1;
}


/* ----- Pop-up construction ----------------------------------------------- */



void add_pop_header(struct gui *gui, unsigned width, const char *label)
{
	struct overlay_style style = {
		.font	= BOLD_FONT,
		.wmin	= width,
		.wmax	= width,
		.radius	= 0,
		.pad	= 0,
		.skip	= 6,
		.fg	= { 0.5, 0.0, 0.0, 1.0 },
		.bg	= { 0.0, 0.0, 0.0, 0.0 },
		.frame	= { 1.0, 1.0, 1.0, 1.0 }, /* debugging */
		.width	= 0,
	};
	struct overlay *over;

	over = overlay_add(&gui->pop_overlays, NULL, NULL, NULL, NULL);
	overlay_text(over, "%s", label);
	overlay_style(over, &style);
}


void add_pop_item(struct gui *gui, void (*click)(void *user), void *user,
    unsigned width, bool dim, const char *fmt, ...)
{
	struct overlay_style style = {
		.font	= BOLD_FONT,
		.wmin	= width,
		.wmax	= width,
		.radius	= 0,
		.pad	= 0,
		.skip	= 4,
		.fg	= { 0.0, 0.0, 0.0, 1.0 },
		.bg	= { 0.0, 0.0, 0.0, 0.0 },
		.frame	= { 1.0, 1.0, 1.0, 1.0 }, /* debugging */
		.width	= 0,
	};
	struct overlay *over;
	va_list ap;

	if (dim)
		style.fg = RGBA(0.5, 0.5, 0.5, 1.0);

	over = overlay_add(&gui->pop_overlays, &gui->aois, NULL, click, user);
	va_start(ap, fmt);
	overlay_vtext(over, fmt, ap);
	va_end(ap);
	overlay_style(over, &style);
}


void add_pop_frame(struct gui *gui)
{
	int w, h;

	overlay_size_all(gui->pop_overlays,
	    gtk_widget_get_pango_context(gui->da), 0, 1, &w, &h);

	struct overlay_style style = {
		.font	= BOLD_FONT,
		.wmin	= w,
		.hmin	= h,
		.radius	= 0,
		.pad	= POP_STACK_PADDING,
		.skip	= 0,
		.fg	= { 0.0, 0.0, 0.0, 1.0 },
		.bg	= { 0.9, 0.9, 0.3, 0.8 },
		.frame	= { 0.0, 0.0, 0.0, 1.0 }, /* debugging */
		.width	= 1,
	};
	struct overlay *over;

	over = overlay_add(&gui->pop_underlays, &gui->aois,
	    pop_hover, NULL, gui);
	overlay_text_raw(over, "");
	overlay_style(over, &style);

	/*
	 * This makes it all work. When we receive a click while hovering, it
	 * goes to the hovering overlay if that overlay accepts clicks.
	 * However, if the overlay accepting the click is different, we first
	 * de-hover.
	 *
	 * Now, in the case of the frame overlay, dehovering would destroy the
	 * destination overlays right before trying to deliver the click.
	 *
	 * We solve this by declaring the frame overlay to be "related" to the
	 * destination overlays. This suppresses dehovering.
	 */
	overlay_set_related_all(gui->pop_overlays, over);
}


void place_pop(struct gui *gui, const struct dwg_bbox *bbox)
{
	const struct gui_sheet *curr_sheet = gui->curr_sheet;
	GtkAllocation alloc;
	int sx, sy, ex, ey, mx, my;

	eeschema_coord(gui,
	    bbox->x - curr_sheet->xmin, bbox->y - curr_sheet->ymin,
	    &sx, &sy);
	eeschema_coord(gui, bbox->x + bbox->w - curr_sheet->xmin,
	    bbox->y + bbox->h - curr_sheet->ymin, &ex, &ey);

	gtk_widget_get_allocation(gui->da, &alloc);
	mx = (sx + ex) / 2;
	my = (sy + ey) / 2;
	if (mx < alloc.width / 2) {
		gui->pop_x = sx - CHEAT;
		gui->pop_dx = 1;
	} else {
		gui->pop_x = ex + CHEAT;
		gui->pop_dx = -1;
	}
	if (my < alloc.height / 2) {
		gui->pop_y = sy - CHEAT;
		gui->pop_dy = 1;
	} else {
		gui->pop_y = ey + CHEAT;
		gui->pop_dy = -1;
	}

}


void place_pop_cover(struct gui *gui, const struct dwg_bbox *bbox,
    struct overlay_style *style)
{
	const struct gui_sheet *curr_sheet = gui->curr_sheet;
	int sx, sy, ex, ey;

	eeschema_coord(gui,
	    bbox->x - curr_sheet->xmin, bbox->y - curr_sheet->ymin,
	    &sx, &sy);
	eeschema_coord(gui, bbox->x + bbox->w - curr_sheet->xmin,
	    bbox->y + bbox->h - curr_sheet->ymin, &ex, &ey);

	gui->pop_x = sx - style->pad;
	gui->pop_y = sy - style->pad;
	style->wmin = style->wmax = ex - sx + 1;
	style->hmin = style->hmax = ey - sy + 1;
	gui->pop_dx = gui->pop_dy = 0;
}
