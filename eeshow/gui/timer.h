/*
 * gui/timer.h - Performance timer
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_TIMER_H
#define	GUI_TIMER_H

#include <cairo/cairo.h>


void timer_start(void);
void timer_show(cairo_t *cr);
void timer_toggle(void);

#endif /* !GUI_TIMER_H */
