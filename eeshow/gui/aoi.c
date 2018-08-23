/*
 * gui/aoi.c - GUI: areas of interest
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
 */

#include <stddef.h>
#include <math.h>
#include <assert.h>

#include "misc/util.h"
#include "gui/aoi.h"


static const struct aoi *hovering = NULL;


struct aoi *aoi_add(struct aoi **aois, const struct aoi *cfg)
{
	struct aoi *new;

	new = alloc_type(struct aoi);
	*new = *cfg;
	new->next = *aois;
	*aois = new;

	return new;
}


void aoi_update(struct aoi *aoi, const struct aoi *cfg)
{
	struct aoi *next = aoi->next;

	*aoi = *cfg;
	aoi->next = next;
}


static bool in_aoi(const struct aoi *aoi, int x, int y)
{
	return x >= aoi->x && x < aoi->x + aoi->w &&
	    y >= aoi->y && y < aoi->y + aoi->h;
}


static bool hover_d(const struct aoi *aoi, bool on,int x, int y)
{
	return aoi->hover(aoi->user, on,
	    x < aoi->x ? -1 : x >= aoi->x + aoi->w ? 1 : 0,
	    y < aoi->y ? -1 : y >= aoi->y + aoi->h ? 1 : 0);
}


/*
 * We need a pointer to the anchor of the AoI list here because dehovering may
 * delete the AoI *aois points to.
 *
 * We could just check if hovering == *aois, but that seems risky, because
 * hover(..., 0) may destroy more than just the AoI being dehovered.
 */

bool aoi_hover(struct aoi *const *aois, int x, int y)
{
	static int last_x = 0;
	static int last_y = 0;
	const struct aoi *aoi;

	if (hovering) {
		if (in_aoi(hovering, x, y))
			return 1;
		hover_d(hovering, 0, x, y);
		hovering = NULL;
	}

	for (aoi = *aois; aoi; aoi = aoi->next)
		if (aoi->hover && in_aoi(aoi, x, y) &&
		    hover_d(aoi, 1, last_x, last_y)) {
			hovering = aoi;
			break;
		}
	last_x = x;
	last_y = y;
	return aoi;
}


static bool need_dehover(const struct aoi *aois, int x, int y)
{
	const struct aoi *aoi;

	if (!hovering)
		return 0;
	if (hovering->click)
		return 0;
	for (aoi = aois; aoi; aoi = aoi->next)
		if (aoi->related == hovering && aoi->click && in_aoi(aoi, x, y))
			return 0;
	return 1;
}


/* Pointer to the anchor needed for the same reason as in aoi_hover. */

bool aoi_click(struct aoi *const *aois, int x, int y)
{
	const struct aoi *aoi;

	if (need_dehover(*aois, x, y))
		aoi_dehover();

	for (aoi = *aois; aoi; aoi = aoi->next)
		if (aoi->click && in_aoi(aoi, x, y)) {
			aoi->click(aoi->user);
			return 1;
		}
	return 0;
}


void aoi_set_related(struct aoi *aoi, const struct aoi *related)
{
	assert(!aoi->related);
	aoi->related = related;
}


void aoi_remove(struct aoi **aois, const struct aoi *aoi)
{
	assert(aoi);
	if (hovering == aoi) {
		aoi->hover(aoi->user, 0, 0, 0);
		hovering = NULL;
	}
	while (*aois && *aois != aoi)
		aois = &(*aois)->next;
	assert(*aois);
	*aois = aoi->next;
	free((void *) aoi);
}


void aoi_dehover(void)
{
	if (hovering)
		hovering->hover(hovering->user, 0, 0, 0);
	hovering = NULL;
}
