/*
 * gfx/misc.c - Helper functions for geometry and attributes
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "misc/diag.h"
#include "gfx/misc.h"


static bool eq(const int m[6], int xx, int xy, int yx, int yy)
{
	return m[1] == xx && m[2] == xy && m[4] == yx && m[5] == yy;
}


unsigned matrix_to_angle(const int m[6])
{
	if (eq(m, 1, 0, 0, -1))
		return 0;
	if (eq(m, 0, -1, -1, 0))
		return 90;
	if (eq(m, -1, 0, 0, 1))
		return 180;
	if (eq(m, 0, 1, 1, 0))
		return 270;

	if (eq(m, -1, 0, 0, -1))
		return 0;
	if (eq(m, 0, 1, -1, 0))	/* x-flipped, 90 deg */
		return 90;
	if (eq(m, 1, 0, 0, 1))	/* x-flipped */
		return 180;
	if (eq(m, 0, -1, 1, 0))	/* x-flipped, 270 deg */
		return 270;

	fatal("unrecognized matrix %d %d %d %d\n", m[1], m[2], m[4], m[5]);
}


bool matrix_is_mirrored(const int m[6])
{
	if (eq(m, 1, 0, 0, -1))
		return 0;
	if (eq(m, 0, -1, -1, 0))
		return 0;
	if (eq(m, -1, 0, 0, 1))
		return 0;
	if (eq(m, 0, 1, 1, 0))
		return 0;

	if (eq(m, -1, 0, 0, -1))
		return 1;
	if (eq(m, 0, 1, -1, 0))
		return 1;
	if (eq(m, 1, 0, 0, 1))
		return 1;
	if (eq(m, 0, -1, 1, 0))
		return 1;

	fatal("unrecognized matrix %d %d %d %d\n", m[1], m[2], m[4], m[5]);
}


int angle_add(int a, int b)
{
	a += b;
	while (a < 0)
		a += 360;
	return a % 360;
}



int rx(int x, int y, int rot)
{
	switch (rot) {
	case 0:
		return x;
	case 90:
		return y;
	case 180:
		return -x;
	case 270:
		return -y;
	default:
		fatal("rotation %d is not implemented", rot);
	}
}


int ry(int x, int y, int rot)
{
	switch (rot) {
	case 0:
		return y;
	case 90:
		return -x;
	case 180:
		return -y;
	case 270:
		return x;
	default:
		fatal("rotation %d is not implemented", rot);
	}
}
