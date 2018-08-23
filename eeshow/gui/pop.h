/*
 * gui/pop.h - Pop-up helper functions
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_POP_H
#define	GUI_POP_H

#include "gui/common.h"


void eeschema_coord(const struct gui *gui, int x, int y, int *rx, int *ry);
void dehover_pop(struct gui *gui);

void add_pop_header(struct gui *gui, unsigned width, const char *label);
void add_pop_item(struct gui *gui, void (*click)(void *user), void *user,
    unsigned width, bool dim, const char *fmt, ...);
void add_pop_frame(struct gui *gui);
void place_pop(struct gui *gui, const struct dwg_bbox *bbox);
void place_pop_cover(struct gui *gui, const struct dwg_bbox *bbox,
    struct overlay_style *style);

#endif /* !GUI_POP_H */
