/*
 * gui/icons.h - Icons
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_ICONS_H
#define	GUI_ICONS_H

#include <cairo/cairo.h>


extern cairo_surface_t *icon_delta;
extern cairo_surface_t *icon_diff;


void icons_init(void);

#endif /* !GUI_ICONS_H */
