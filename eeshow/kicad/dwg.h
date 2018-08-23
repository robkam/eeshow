/*
 * kicad/dwg.h - Complex drawing functions for KiCad items
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_DWG_H
#define	KICAD_DWG_H

#include "gfx/gfx.h"
#include "gfx/text.h"


enum dwg_shape {
	dwg_unspec,	// UnSpc
	dwg_in,		// Input
	dwg_out,	// Output
	dwg_tri,	// 3State
	dwg_bidir,	// Bidirectional
};

struct dwg_bbox {
	int x, y;
	int w, h;
};


void dwg_label(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox);
void dwg_hlabel(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox);
void dwg_glabel(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox);
void dwg_text(struct gfx *gfx, int x, int y, const char *s, int dir, int dim,
    enum dwg_shape shape, enum text_style style, struct dwg_bbox *bbox);

void dwg_junction(struct gfx *gfx, int x, int y);
void dwg_noconn(struct gfx *gfx, int x, int y);

void dwg_line(struct gfx *gfx, int sx, int sy, int ex, int ey);

void dwg_wire(struct gfx *gfx, int sx, int sy, int ex, int ey);
void dwg_bus(struct gfx *gfx, int sx, int sy, int ex, int ey);

#endif /* !KICAD_DWG_H */
