/*
 * kicad/lib.h - Parse Eeschema .lib file
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_LIB_H
#define KICAD_LIB_H

#include <stdbool.h>

#include "file/file.h"
#include "gfx/text.h"
#include "gfx/gfx.h"
#include "kicad/ext.h"


enum pin_shape {
	pin_inverted		= 1 << 0,
	pin_clock		= 1 << 1,
	pin_input_low		= 1 << 2,
	pin_output_low		= 1 << 3,
	pin_falling_edge	= 1 << 4,
	pin_non_logic		= 1 << 5,
	pin_invisible		= 1 << 6,
};

enum lib_state {
	lib_skip,	/* before a definition */
	lib_def,	/* in definition */
	lib_draw,	/* in drawings */
};

struct lib_obj {
	enum lib_obj_type {
		lib_obj_poly,
		lib_obj_rect,
		lib_obj_circ,
		lib_obj_arc,
		lib_obj_text,
		lib_obj_pin,
	} type;
	unsigned unit;
	unsigned convert;
	union {
		struct lib_poly {
			int thick;
			char fill;
			int points;
			int *x;
			int *y;
		} poly;
		struct lib_rect {
			int thick;
			char fill;
			int sx, sy;
			int ex, ey;
		} rect;
		struct lib_circ {
			int x, y;
			int r;
			int thick;
			char fill;
		} circ;
		struct lib_arc {
			int x, y;
			int r;
			int start_a, end_a;
			int thick;
			char fill;
		} arc;
		struct lib_text {
			int orient;
			int x, y;
			int dim;
			char *s;
			enum text_style style;
			char hor_align;
			char vert_align;
		} text;
		struct lib_pin {
			char *name;
			char *number;
			int x, y;
			int length;
			char orient;
			int number_size;
			int name_size;
			char etype;
			enum pin_shape shape;
		} pin;
	} u;
	struct lib_obj *next;
};

struct comp_alias {
	const char *name;
	struct comp_alias *next;
};

struct comp {
	const char *name;
	struct comp_alias *aliases;
	unsigned units;

	unsigned visible;	/* visible fields, bit mask */
	bool show_pin_name;
	bool show_pin_num;
	unsigned name_offset;

	struct lib_obj *objs;
	struct comp *next;
};

struct lib {
    char* namespace;
	enum lib_state state;

	struct comp *comps;

	struct comp *curr_comp; /* current component */
	struct comp **next_comp;
	struct lib_obj **next_obj;
};


extern struct comp *comps;


const struct comp *lib_find(const struct lib *lib, const char *name);
void lib_render(const struct comp *comp, struct gfx *gfx,
    unsigned unit, unsigned convert, const int m[6]);

bool lib_parse_file(struct lib *lib, struct file *file);
bool lib_parse(struct lib *lib, const char *name, const struct file *related);
bool lib_find_file(struct file *file, const char *name,
    const struct file_names *fn, const struct file *related);
bool lib_parse_search(struct lib *lib, const char *name,
    const struct file_names *fn, const struct file *related);
void lib_init(struct lib *lib);
void lib_free(struct lib *lib);

#endif /* !KICAD_LIB_H */
