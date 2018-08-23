/*
 * gui/style.c - GUI: overlay styles
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "gui/style.h"


#define	OVER_BORDER	8
#define	OVER_RADIUS	6
#define	OVER_SEP	8

#define	NORMAL_PAD	8
#define	NORMAL_RADIUS	6
#define	NORMAL_SKIP	8
#define	NORMAL_WIDTH	2

#define	DENSE_PAD	4
#define	DENSE_RADIUS	3
#define	DENSE_SKIP	5
#define	DENSE_WIDTH	1

#define	BG_STANDARD		{ 0.8, 0.9, 1.0, 0.8 }
#define	FG_STANDARD		{ 0.0, 0.0, 0.0, 1.0 }
#define	FRAME_STANDARD		{ 0.5, 0.5, 1.0, 0.7 }

#define	BG_DIFF_NEW		BG_STANDARD
#define	FG_DIFF_NEW		{ 0.0, 0.6, 0.0, 1.0 }
#define	FRAME_DIFF_NEW		FRAME_STANDARD

#define	BG_DIFF_OLD		BG_STANDARD
#define	FG_DIFF_OLD		{ 0.8, 0.0, 0.0, 1.0 }
#define	FRAME_DIFF_OLD		FRAME_STANDARD


#define	BOX_ATTRS(style)		\
	.pad	= style##_PAD,		\
	.radius	= style##_RADIUS,	\
	.skip	= style##_SKIP,		\
	.width	= style##_WIDTH

#define	NORMAL	BOX_ATTRS(NORMAL)
#define	DENSE	BOX_ATTRS(DENSE)

#define	COLOR_ATTRS(style)		\
	.bg	= BG_##style,		\
	.fg	= FG_##style,		\
	.frame	= FRAME_##style

#define	STANDARD		COLOR_ATTRS(STANDARD)
#define	DIFF_NEW		COLOR_ATTRS(DIFF_NEW)
#define	DIFF_OLD		COLOR_ATTRS(DIFF_OLD)


struct overlay_style overlay_style_default = {
	.font	= NORMAL_FONT,
	NORMAL,
	STANDARD,
}, overlay_style_dense = {
	.font	= NORMAL_FONT,
	DENSE,
	STANDARD,
}, overlay_style_diff_new = {
	.font	= NORMAL_FONT,
	NORMAL,
	DIFF_NEW,
}, overlay_style_diff_old = {
	.font	= NORMAL_FONT,
	NORMAL,
	DIFF_OLD,
};
