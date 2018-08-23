/*
 * gfx/misc.h - Helper functions for geometry and attributes
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef GFX_MISC_H
#define GFX_MISC_H

#include <stdbool.h>


static inline int mxr(int x, int y, const int m[6])
{
	return x * m[1] + y * m[2];
}


static inline int myr(int x, int y, const int m[6])
{
	return x * m[4] + y * m[5];
}


static inline int mx(int x, int y, const int m[6])
{
	return m[0] + mxr(x, y, m);
}


static inline int my(int x, int y, const int m[6])
{
	return m[3] + myr(x, y, m);
}


unsigned matrix_to_angle(const int m[6]);
bool matrix_is_mirrored(const int m[6]);
int angle_add(int a, int b);

int rx(int x, int y, int rot);
int ry(int x, int y, int rot);

#endif /* !GFX_MISC_H */
