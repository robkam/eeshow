/*
 * kicad/sch.h - Parse Eeschema .sch file
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_SCH_H
#define KICAD_SCH_H

#include <stdbool.h>

#include "kicad/dwg.h"
#include "gfx/text.h"
#include "gfx/gfx.h"
#include "file/file.h"
#include "kicad/lib.h"


enum sch_state {
	sch_basic,	/* basic state */
	sch_descr,	/* prelude and description */
	sch_comp,	/* component */
	sch_sheet,	/* sub-sheet */
	sch_text,	/* text or label */
	sch_wire,	/* wire */
	sch_eof,	/* skipping anything after $EndSCHEMATC */
};

struct sheet;

struct sch_obj {
	enum sch_obj_type {
		sch_obj_wire,
		sch_obj_junction,
		sch_obj_noconn,
		sch_obj_text,
		sch_obj_glabel,
		sch_obj_comp,
		sch_obj_sheet,
	} type;

	int x, y;

	union {
		struct sch_wire {
			void (*fn)(struct gfx *gfx,
			    int sx, int sy, int ex, int ey);
			int ex, ey;
		} wire;
		struct sch_text {
			void (*fn)(struct gfx *gfx,
			    int x, int y, const char *s,
			    int dir, int dim, enum dwg_shape shape,
			    enum text_style style, struct dwg_bbox *bbox);
			const char *s;
			int dir;	/* orientation */
			int dim;	/* dimension */
			enum dwg_shape shape;
			enum text_style style;
			struct dwg_bbox bbox; /* set when rendering; glabel */
		} text;
		struct sch_comp {
			const struct comp *comp; /* current component */
			unsigned unit;	/* unit of current component */
			unsigned convert;/* "De Morgan"  selection */
			struct comp_field {
				struct text txt;
				unsigned n; /* field number */
				bool visible;
				struct comp_field *next;
			} *fields;
			int m[6];
		} comp;
		struct sch_sheet {
			unsigned h, w;
			const char *name;
			unsigned name_dim;
			const char *file;
			unsigned file_dim;
			bool rotated;
			bool error;	/* if set, sheet == NULL */
			const struct sheet *sheet;
				/* pointer to sub-sheet; NULL if absent */

			struct sheet_field {
				char *s;
				int x, y;
				unsigned dim;
				enum dwg_shape shape;
				unsigned side;
				struct sheet_field *next;
			} *fields;
		} sheet;
	} u;

	struct sch_obj *next;
};

struct sheet {
	const char *title;		/* malloced, unless delta */
	const char *file;		/* file name; idem */
	const char *path;		/* schematics path; idem */
	struct sch_obj *objs;
	struct sch_obj **next_obj;
	struct sheet *next;

	time_t mtime;			/* file / commit time */

	/* header items */
	const char *size;		/* paper size, from $Descr */
	int w, h;
	const char *date;
	const char *rev;
	const char *comp;
	const char **comments;
	unsigned n_comments;

	bool has_children;		/* aka sub-sheets */

	/* caching */
	void *oid;
};

struct sch_ctx {
	enum sch_state state;

	bool recurse;

	struct sch_obj obj;

	struct sheet *curr_sheet;
	struct sheet *sheets;
	struct sheet **next_sheet;

	/* for caching */
	const struct sch_ctx *prev;

	const struct lib *lib;
};


void decode_alignment(struct text *txt, char hor, char vert);

void sch_render(const struct sheet *sheet, struct gfx *gfx);
bool sch_parse(struct sch_ctx *ctx, struct file *file, const struct lib *lib,
    const struct sch_ctx *prev);
void sch_init(struct sch_ctx *ctx, bool recurse);
void sch_free(struct sch_ctx *ctx);

#endif /* !KICAD_SCH_H */
