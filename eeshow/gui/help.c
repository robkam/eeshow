/*
 * gui/help.c - Online help
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

#include <gtk/gtk.h>

#include "gui/view.h"
#include "gui/help.h"


static void key_press(void *user, struct view *view, guint keyval)
{
	switch (keyval) {
	case GDK_KEY_h:
	case GDK_KEY_Help:
	case GDK_KEY_q:
	case GDK_KEY_Escape:
		view_visible(view, 0);
		break;
	}
}


void help(void)
{
	static struct view *view = NULL;

	if (!view || !view_visible_toggle(view)) {
		if (view)
			view_close(view);
		view = view_open(key_press, NULL);
		view_update(view,
#include "../help.inc"
		    , 1);
	}
}
