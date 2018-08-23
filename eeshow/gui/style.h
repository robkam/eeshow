/*
 * gui/style.h - GUI: overlay styles
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_STYLE_H
#define	GUI_STYLE_H

#include "gui/over.h"


#define	NORMAL_FONT	"Helvetica 10"
#define	BOLD_FONT	"Helvetica Bold 10"
#define	BOLD_FONT_LARGE	"Helvetica Bold 12"

#define	FRAME_SEL_ONLY	0.0, 0.0, 0.0, 0.9
#define	FRAME_SEL_OLD	0.8, 0.2, 0.2, 0.9
#define	FRAME_SEL_NEW	0.0, 0.6, 0.0, 0.9

#define	BG_NEW		0.6, 1.0, 0.6, 0.8
#define	BG_OLD		1.0, 0.8, 0.8, 0.8


#define RGBA(r, g, b, a)	((struct color) { (r), (g), (b), (a) })
#define COLOR(color)		RGBA(color)


extern struct overlay_style overlay_style_default;
extern struct overlay_style overlay_style_dense;
extern struct overlay_style overlay_style_diff_new;
extern struct overlay_style overlay_style_diff_old;

#endif /* !GUI_STYLE_H */
