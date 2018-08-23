/*
 * gui/aoi.h - GUI: areas of interest
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_AOI_H
#define	GUI_AOI_H

#include <stdbool.h>


struct aoi {
	int x, y, w, h;		/* activation box, eeschema coordinates */
				/* points to hovered aoi, or NULL */

	bool (*hover)(void *user, bool on, int dx, int dy);
	void (*click)(void *user);
	void *user;

	const struct aoi *related; /* considered equal for clicks */

	struct aoi *next;
};


struct aoi *aoi_add(struct aoi **aois, const struct aoi *cfg);
void aoi_update(struct aoi *aoi, const struct aoi *cfg);
bool aoi_hover(struct aoi *const *aois, int x, int y);

bool aoi_click(struct aoi *const *aois, int x, int y);

void aoi_set_related(struct aoi *aoi, const struct aoi *related);

void aoi_remove(struct aoi **aois, const struct aoi *aoi);
void aoi_dehover(void);

#endif /* !GUI_AOI_H */
