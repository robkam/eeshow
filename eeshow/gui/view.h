/*
 * gui/view.h - Text view window
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_VIEW_H
#define	GUI_VIEW_H

#include <stdbool.h>


struct view;

struct view *view_open(
    void (*key_press)(void *user, struct view *view, guint keyval), void *user);
bool view_visible(struct view *view, bool visible);
bool view_visible_toggle(struct view *view);
void view_close(struct view *view);
bool view_update(struct view *view, const char *s, bool markup);

#endif /* !GUI_VIEW_H */
