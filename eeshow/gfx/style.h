/*
 * gfx/style.h - Drawing style
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GFX_STYLE_H
#define	GFX_STYLE_H

#include <stdint.h>


/* FIG colors */

#define	COLOR_NONE		-1
#define	COLOR_BLACK		0
#define	COLOR_BLUE		1
#define	COLOR_GREEN		2
#define	COLOR_CYAN		3
#define	COLOR_RED		4
#define	COLOR_YELLOW		6
#define	COLOR_WHITE		7
#define	COLOR_GREEN4		12
#define	COLOR_GREEN3		13
#define	COLOR_GREEN2		14
#define	COLOR_CYAN4		15
#define	COLOR_CYAN3		16
#define	COLOR_RED4		18
#define	COLOR_RED3		19
#define	COLOR_MAGENTA4		21
#define	COLOR_BROWN2		26
#define	COLOR_PINK4		27

#define	COLOR_DARK_YELLOW	32	/* user-defined */
#define	COLOR_LIGHT_GREY	33	/* user-defined, not used by FIG */
#define	COLOR_LIGHT_YELLOW	34	/* user-defined */
#define	COLOR_ORANGE		35	/* user-defined */

#define	COLOR_COMP_DWG		COLOR_RED4
#define	COLOR_COMP_DWG_BG	COLOR_LIGHT_YELLOW
#define	COLOR_SHEET_DWG		COLOR_BLUE
#define	COLOR_TEXT		COLOR_BLUE
#define	COLOR_WIRE		COLOR_GREEN4
#define	COLOR_BUS		COLOR_BLUE
#define	COLOR_NOCONN		COLOR_BLUE
#define	COLOR_GLABEL		COLOR_RED4
#define	COLOR_HLABEL		COLOR_DARK_YELLOW
#define	COLOR_HSHEET_BOX	COLOR_MAGENTA4
#define	COLOR_HSHEET_SHEET	COLOR_FIELD
#define	COLOR_HSHEET_FILE	COLOR_HLABEL
#define	COLOR_LABEL		COLOR_BLACK
#define	COLOR_FIELD		COLOR_CYAN4
#define	COLOR_PIN_NAME		COLOR_FIELD
#define	COLOR_PIN_NUMBER	COLOR_RED4
#define	COLOR_PIN_EXTRA		COLOR_ORANGE
#define	COLOR_MISSING_FG	COLOR_RED
#define	COLOR_MISSING_BG	COLOR_PINK4

#define	FONT_HELVETICA		16
#define	FONT_HELVETICA_OBLIQUE	17
#define	FONT_HELVETICA_BOLD	18
#define	FONT_HELVETICA_BOLDOB	19

#define	LAYER_GLABEL		20
#define	LAYER_HLABEL		LAYER_GLABEL
#define	LAYER_LABEL		LAYER_GLABEL
#define	LAYER_TEXT		30
#define	LAYER_NOCONN		40
#define	LAYER_WIRES		50
#define	LAYER_BUSSES		LAYER_WIRES
#define	LAYER_FIELD		60
#define	LAYER_PIN_NAME		LAYER_FIELD
#define	LAYER_PIN_NUMBER	LAYER_FIELD
#define	LAYER_HSHEET_FIELD	LAYER_FIELD
#define	LAYER_HSHEET_BOX	70
#define	LAYER_LINES		100
#define	LAYER_COMP_DWG		120
#define	LAYER_PIN_EXTRA		150
#define	LAYER_COMP_DWG_BG	200

#define	WIDTH_WIRE		2
#define	WIDTH_BUS		WIDTH_WIRE
#define	WIDTH_LINE		2
#define	WIDTH_COMP_DWG		2

#define JUNCTION_R		30

#define	NOCONN_LEN		25

#define	LABEL_OFFSET		15	// eeschema has more like 10
#define	GLABEL_OFFSET		20
#define	HLABEL_OFFSET_F		0.4	// * text size
#define	PIN_NUM_OFFSET		15	// eeschema has more like 10
#define	HSHEET_FIELD_OFFSET	10

#define	NEWLINE_SKIP		1.4	// * text size

#define	MISSING_WIDTH		300	// size of missing things indicator box
#define	MISSING_HEIGHT		300

#define	PIN_R			25	/* radius and feature step */
#define	PIN_X_R			13	/* feature step for "NC" */
#define	PIN_EXTRA_R		25	/* feature step */


extern uint32_t color_rgb[];
extern unsigned n_color_rgb;

#endif /* !GFX_STYLE_H */
