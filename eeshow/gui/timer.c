/*
 * gui/timer.c - Performance timer
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
#include <stdio.h>
#include <sys/time.h>

#include <cairo/cairo.h>

#include "misc/util.h"
#include "gui/timer.h"


static bool timer_enabled = 0;
static struct timeval then;


void timer_start(void)
{
	if (timer_enabled)
		gettimeofday(&then, NULL);
}


static void timer_show_cairo(cairo_t *cr, const char *s)
{
	double x1, y1, x2, y2;
	cairo_text_extents_t ext;

	cairo_reset_clip(cr);
	cairo_clip_extents(cr, &x1, &y1, &x2, &y2);
	cairo_select_font_face(cr, "Mono Sans",
	    CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 20);
	cairo_text_extents(cr, s, &ext);
	cairo_move_to(cr, x2 - ext.width - 4, y2 - 4);
	cairo_set_source_rgb(cr, 0, 0, 0.7);
	cairo_show_text(cr, s);
}


void timer_show(cairo_t *cr)
{
	struct timeval now;
	char *s;

	if (!timer_enabled)
		return;

	gettimeofday(&now, NULL);
	now.tv_sec -= then.tv_sec;
	now.tv_usec -= then.tv_usec;
	if (now.tv_usec < 0) {
		now.tv_sec--;
		now.tv_usec += 1000000;
	}

	alloc_printf(&s , "%u.%03d",
	    (unsigned) now.tv_sec, (int) now.tv_usec / 1000);
	timer_show_cairo(cr, s);
}


void timer_toggle(void)
{
	timer_enabled = !timer_enabled;
	timer_start();
}
