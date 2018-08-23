/*
 * kicad/sch-render.c - Render schematics
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/misc.h"
#include "gfx/style.h"
#include "gfx/gfx.h"
#include "kicad/dwg.h"
#include "kicad/lib.h"
#include "kicad/sch.h"


/* ----- Rendering --------------------------------------------------------- */


static void dump_field(const struct comp_field *field, struct gfx *gfx,
    const int m[6])
{
	struct text txt = field->txt;
	int dx, dy;

	dx = txt.x - m[0];
	dy = txt.y - m[3];
	txt.x = mx(dx, dy, m);
	txt.y = my(dx, dy, m);

	text_rot(&txt, matrix_to_angle(m));

	switch (txt.rot) {
	case 180:
		text_rot(&txt, 180);
		txt.hor = text_flip(txt.hor);
		txt.vert = text_flip(txt.vert);
		break;
	case 270:
		text_rot(&txt, 180);
		txt.vert = text_flip(txt.vert);
		txt.hor = text_flip(txt.hor);
		break;
	default:
		break;
	}

	if (matrix_is_mirrored(m)) {
		if ((txt.rot % 180) == 0)
			txt.hor = text_flip(txt.hor);
		else
			txt.vert = text_flip(txt.vert);
	}

	text_show(&txt, gfx, COLOR_FIELD, LAYER_FIELD);
}


static void do_hsheet_text(const struct sch_obj *obj,
    const struct sch_sheet *sheet, struct gfx *gfx)
{
	char *s;

	assert(sheet->name && sheet->file);

	struct text sheet_txt = {
		.size	= sheet->name_dim,
		.x	= obj->x,
		.y	= obj->y,
		.rot	= 0,
		.hor	= text_min,
		.vert	= text_min,
		.style	= text_normal,
	};
	alloc_printf(&s, "Sheet: %s", sheet->name);
	sheet_txt.s = s; /* work around "const" mismatch */

	struct text file_txt = {
		.size	= sheet->file_dim,
		.x	= obj->x,
		.y	= obj->y,
		.rot	= 0,
		.hor	= text_min,
		.vert	= text_max,
		.style	= text_normal,
	};
	alloc_printf(&s, "File: %s", sheet->file);
	file_txt.s = s; /* work around "const" mismatch */

	if (sheet->rotated) {
		sheet_txt.rot = file_txt.rot = 90;
		sheet_txt.x -= HSHEET_FIELD_OFFSET;
		sheet_txt.y += sheet->h;
		file_txt.x += sheet->w + HSHEET_FIELD_OFFSET;
		file_txt.y += sheet->h;
	} else {
		sheet_txt.y -= HSHEET_FIELD_OFFSET;
		file_txt.y += sheet->h + HSHEET_FIELD_OFFSET;
	}

	text_show(&sheet_txt, gfx, COLOR_HSHEET_SHEET, LAYER_HSHEET_FIELD);
	text_show(&file_txt, gfx, COLOR_HSHEET_FILE, LAYER_HSHEET_FIELD);

	free((void *) sheet_txt.s);
	free((void *) file_txt.s);
}


static void render_comp(const struct sch_comp *comp, struct gfx *gfx)
{
	const struct comp_field *field;

	lib_render(comp->comp, gfx, comp->unit, comp->convert, comp->m);
	for (field = comp->fields; field; field = field->next) {
		if (!field->visible) {
			if (field->n == 0)
				gfx_set_extra(gfx, gfx_reference);
			else if (field->n == 2)
				gfx_set_extra(gfx, gfx_footprint);
			else
				continue;
		}
		dump_field(field, gfx, comp->m);
		gfx_set_extra(gfx, 0);
	}
}


static void render_sheet(const struct sch_obj *obj,
    const struct sch_sheet *sheet, struct gfx *gfx)
{
	const struct sheet_field *field;

	gfx_rect(gfx, obj->x, obj->y, obj->x + sheet->w, obj->y + sheet->h,
	    COLOR_HSHEET_BOX, sheet->error ? COLOR_MISSING_BG : COLOR_NONE,
	    LAYER_HSHEET_BOX);
	do_hsheet_text(obj, sheet, gfx);

	for (field = sheet->fields; field; field = field->next)
		dwg_hlabel(gfx, field->x, field->y, field->s,
		    field->side, field->dim,
		    field->shape, text_normal, NULL);
	// free(field->s)
}


void sch_render(const struct sheet *sheet, struct gfx *gfx)
{
	struct sch_obj *obj;

	for (obj = sheet->objs; obj; obj = obj->next)
		switch (obj->type) {
		case sch_obj_wire:
			{
				const struct sch_wire *wire = &obj->u.wire;

				wire->fn(gfx, obj->x, obj->y,
				    wire->ex, wire->ey);
			}
			break;
		case sch_obj_junction:
			dwg_junction(gfx, obj->x, obj->y);
			break;
		case sch_obj_noconn:
			dwg_noconn(gfx, obj->x, obj->y);
			break;
		case sch_obj_glabel:
		case sch_obj_text:
			{
				struct sch_text *text = &obj->u.text;

				text->fn(gfx, obj->x, obj->y,
				    text->s, text->dir,
				    text->dim, text->shape, text->style,
				    &text->bbox);
			}
			break;
		case sch_obj_comp:
			render_comp(&obj->u.comp, gfx);
			break;
		case sch_obj_sheet:
			render_sheet(obj, &obj->u.sheet, gfx);
			break;
		default:
			BUG("invalid object type \"%d\"", obj->type);
		}
}
