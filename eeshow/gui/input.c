/*
 * gui/input.c - Input operations
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
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include <gtk/gtk.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gui/timer.h"
#include "gui/input.h"


#define	DRAG_RADIUS	5


static struct input {
	const struct input_ops *ops;
	void *user;

	enum state {
		input_idle,
		input_clicking,
		input_ignoring,	/* click rejected by moving the cursor */
		input_hovering,
		input_hovering_down, /* mouse button is pressed */
		input_dragging,
	} state;

	struct input *next;
} *sp = NULL;

static int buttons = 0;
static int curr_x, curr_y;		/* last mouse position */
static double clicked_x, clicked_y;	/* button down position */


/* ----- Debugging tools --------------------------------------------------- */


static const char *state(void)
{
	switch (sp->state) {
	case input_idle:
		return "IDLE";
	case input_clicking:
		return "CLICKING";
	case input_ignoring:
		return "IGNORING";
	case input_hovering:
		return "HOVERING";
	case input_hovering_down:
		return "HOVERING_DOWN";
	case input_dragging:
		return "DRAGGING";
	default:
		BUG("invalid state %d", sp->state);
	}
}


/* ----- Shared operations ------------------------------------------------- */


static bool begin_drag(const GdkEventMotion *event)
{
	const struct input *old_sp = sp;

	if (hypot(event->x - clicked_x, event->y - clicked_y) < DRAG_RADIUS)
		return 0;
	if (sp->ops->drag_begin &&
	    sp->ops->drag_begin(sp->user, clicked_x, clicked_y))
		sp->state = input_dragging;
	else
		sp->state = input_ignoring;
	assert(sp == old_sp);
	return 1;
}


static void hover_consider(int x, int y)
{
	const struct input *old_sp = sp;

	assert(sp->state == input_idle);

	if (sp->ops->hover_begin && sp->ops->hover_begin(sp->user, x, y))
		sp->state = input_hovering;
	assert(sp == old_sp);
}


static void hover_update(int x, int y)
{
	const struct input *old_sp = sp;

	assert(sp->state == input_hovering || sp->state == input_hovering_down);

	if (!sp->ops->hover_update)
		return;

	/*
	 * Caution: hover_update may switch input layers. If this happens,
	 * hovering was already ended when cleaning up the old input layer.
	 */
	if (sp->ops->hover_update(sp->user, x, y))
		return;
	if (sp != old_sp)
		return;

	progress(3, "hover_update %s", state());

	switch (sp->state) {
	case input_idle:
	case input_hovering:
	case input_ignoring:
		sp->state = input_idle;
		break;
	case input_clicking:
	case input_hovering_down:
		sp->state = input_clicking;
		break;
	case input_dragging:
	default:
		BUG("invalid state %d", sp->state);
	}

	if (sp->ops->hover_end)
		sp->ops->hover_end(sp->user);
}


/* ----- Indirect update --------------------------------------------------- */

/*
 * Geometry changes may require a reassessment of the hover situation. This is
 * roughly equivalent to what we would do on a mouse movement over distance
 * zero.
 */

void input_update(void)
{
	switch (sp->state) {
	case input_idle:
		hover_consider(curr_x, curr_y);
		break;
	case input_hovering:
		hover_update(curr_x, curr_y);
		break;
	case input_clicking:
	case input_ignoring:
	case input_hovering_down:
	case input_dragging:
		break;
	default:
		BUG("invalid state %d", sp->state);
	}
}


/* ----- Mouse button ------------------------------------------------------ */


static gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event,
    gpointer data)
{
	curr_x = event->x;
	curr_y = event->y;

	if (!sp)
		return TRUE;

	progress(3, "motion %s", state());

	timer_start();

	switch (sp->state) {
	case input_idle:
		hover_consider(event->x, event->y);
		break;
	case input_clicking:
		begin_drag(event);
		break;
	case input_ignoring:
		break;
	case input_hovering_down:
		if (begin_drag(event)) {
			if (sp->ops->hover_end)
				sp->ops->hover_end(sp->user);
			break;
		}
		/* fall through */
	case input_hovering:
		hover_update(event->x, event->y);
		break;
	case input_dragging:
		if (sp->ops->drag_move)
			sp->ops->drag_move(sp->user,
			    event->x - clicked_x, event->y - clicked_y);
		clicked_x = event->x;
		clicked_y = event->y;
		break;
	default:
		BUG("invalid state %d", sp->state);
	}
	return TRUE;
}


static gboolean button_press_event(GtkWidget *widget, GdkEventButton *event,
    gpointer data)
{
	if (event->button > 2)
		return TRUE;

	progress(3, "press %s", state());

	timer_start();

	buttons |= 1 << event->button;

	switch (sp->state) {
	case input_idle:
		sp->state = input_clicking;
		clicked_x = event->x;
		clicked_y = event->y;
		break;
	case input_clicking:
	case input_ignoring:
	case input_dragging:
	case input_hovering_down:
		/* ignore double-click */
		break;
	case input_hovering:
		sp->state = input_hovering_down;
		clicked_x = event->x;
		clicked_y = event->y;
		break;
	default:
		BUG("invalid state %d", sp->state);
	}

	return TRUE;
}


static gboolean button_release_event(GtkWidget *widget, GdkEventButton *event,
    gpointer data)
{
	const struct input *old_sp = sp;

	if (event->button > 2)
		return TRUE;

	progress(3, "release %s", state());

	timer_start();

	buttons &= ~(1 << event->button);
	if (buttons)
		return TRUE;

	switch (sp->state) {
	case input_idle:
		/* hover_click changed the input configuration */
		break;
	case input_clicking:
		sp->state = input_idle;
		if (sp->ops->click)
			sp->ops->click(sp->user, clicked_x, clicked_y);
		break;
	case input_ignoring:
		sp->state = input_idle;
		break;
	case input_dragging:
		sp->state = input_idle;
		if (sp->ops->drag_end)
			sp->ops->drag_end(sp->user);
		break;
	case input_hovering:
		break;
	case input_hovering_down:
		if (sp->ops->hover_click &&
		    sp->ops->hover_click(sp->user, event->x, event->y) &&
		    sp == old_sp) {
			sp->state = input_idle;
			if (sp->ops->hover_end)
				sp->ops->hover_end(sp->user);
		}
		break;
	default:
		BUG("invalid state %d", sp->state);
	}

	return TRUE;
}


/* ----- Scroll wheel ------------------------------------------------------ */


static gboolean scroll_event(GtkWidget *widget, GdkEventScroll *event,
    gpointer data)
{
	timer_start();

	if (!sp || !sp->ops->scroll)
		return TRUE;
	switch (event->direction) {
	case GDK_SCROLL_UP:
		sp->ops->scroll(sp->user, event->x, event->y, -1);
		break;
	case GDK_SCROLL_DOWN:
		sp->ops->scroll(sp->user, event->x, event->y, 1);
		break;
	default:
		/* ignore */;
	}
	return TRUE;
}


/* ----- Keys -------------------------------------------------------------- */


static gboolean key_press_event(GtkWidget *widget, GdkEventKey *event,
    gpointer data)
{
	timer_start();

	if (sp && sp->ops->key)
		sp->ops->key(sp->user, curr_x, curr_y, event->keyval);
	return TRUE;
}


/* ----- Covenience function for hover_begin and drag_begin ---------------- */


bool input_accept(void *user, int x, int y)
{
	return 1;
}


/* ----- Adding/removing interaction layers -------------------------------- */


static void cleanup(void)
{
	if (!sp)
		return;

	switch (sp->state) {
	case input_hovering:
		if (sp->ops->hover_end)
			sp->ops->hover_end(sp->user);
		break;
	case input_dragging:
		if (sp->ops->drag_end)
			sp->ops->drag_end(sp->user);
		break;
	default:
		;
	}

	sp->state = input_idle;
}


void input_push(const struct input_ops *ops, void *user)
{
	struct input *new;

	cleanup();

	new = alloc_type(struct input);
	new->ops = ops;
	new->user = user;
	new->state = input_idle;
	new->next = sp;
	sp = new;
}


void input_pop(void)
{
	struct input *next = sp->next;

	cleanup();
	free(sp);
	sp = next;
}


/* ----- Initialization ---------------------------------------------------- */


void input_setup(GtkWidget *da)
{
	gtk_widget_set_can_focus(da, TRUE);

	gtk_widget_add_events(da,
	    GDK_KEY_PRESS_MASK |
	    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
	    GDK_SCROLL_MASK |
	    GDK_POINTER_MOTION_MASK);

	g_signal_connect(G_OBJECT(da), "key_press_event",
	    G_CALLBACK(key_press_event), NULL);
	g_signal_connect(G_OBJECT(da), "motion_notify_event",
	    G_CALLBACK(motion_notify_event), NULL);
	g_signal_connect(G_OBJECT(da), "button_press_event",
	    G_CALLBACK(button_press_event), NULL);
	g_signal_connect(G_OBJECT(da), "button_release_event",
	    G_CALLBACK(button_release_event), NULL);
	g_signal_connect(G_OBJECT(da), "scroll_event",
	    G_CALLBACK(scroll_event), NULL);
}
