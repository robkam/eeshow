/*
 * gui/input.h - Input operations
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_INPUT_H
#define	GUI_NPUT_H

#include <stdbool.h>

#include <gtk/gtk.h>


/*
 * All members of input_ops are optional, i.e., can be NULL.
 *
 * hover_begin and drag_begin must not call input_push or input_pop.
 */

struct input_ops {
	bool (*click)(void *user, int x, int y);

	bool (*hover_begin)(void *user, int x, int y);
	bool (*hover_update)(void *user, int x, int y);
	bool (*hover_click)(void *user, int x, int y);
	void (*hover_end)(void *user);

	bool (*drag_begin)(void *user, int x, int y);
	void (*drag_move)(void *user, int dx, int dy);
	void (*drag_end)(void *user);

	void (*scroll)(void *user, int x, int y, int dy);
		/* down = 1, up = -1 */

	void (*key)(void *user, int x, int y, int keyval);
};


bool input_accept(void *user, int x, int y);

void input_update(void);

void input_push(const struct input_ops *ops, void *user);
void input_pop(void);
void input_setup(GtkWidget *da);

#endif /* !GUI_INPUT_H */
