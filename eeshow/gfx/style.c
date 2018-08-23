/*
 * gfx/style.c - Drawing style
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdint.h>

#include "misc/util.h"
#include "gfx/style.h"


uint32_t color_rgb[] = {
	[COLOR_BLACK]		= 0x000000,
	[COLOR_BLUE]		= 0x0000ff,
	[COLOR_GREEN]		= 0x00ff00,
	[COLOR_CYAN]		= 0x00ffff,
	[COLOR_RED]		= 0xff0000,
	[COLOR_YELLOW]		= 0xffff00,
	[COLOR_WHITE]		= 0xffffff,
	[COLOR_GREEN4]		= 0x009000,
	[COLOR_GREEN3]		= 0x00b000,
	[COLOR_GREEN2]		= 0x00d000,
	[COLOR_CYAN4]		= 0x009090,
	[COLOR_CYAN3]		= 0x00b0b0,
	[COLOR_RED4]		= 0x900000,
	[COLOR_RED3]		= 0xb00000,
	[COLOR_MAGENTA4]	= 0x900090,
	[COLOR_BROWN2]		= 0xc06000,
	[COLOR_PINK4]		= 0xff8080,

	/* user-defined colors */
	[COLOR_DARK_YELLOW]	= 0x848400,
	[COLOR_LIGHT_GREY]	= 0xd0d0d0,
	[COLOR_LIGHT_YELLOW]	= 0xffffc2,
		/*
		 * If COLOR_LIGHT_YELLOW should be visible in diff (and not
		 * just appear white), use 0xffffa0 or darker.
		 */
	[COLOR_ORANGE]		= 0xff6000,
};

unsigned n_color_rgb = ARRAY_ELEMENTS(color_rgb);
