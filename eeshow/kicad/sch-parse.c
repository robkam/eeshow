/*
 * kicad/sch-parse.c - Parse Eeschema .sch file
 *
 * Written 2016-2017 by Werner Almesberger
 * Copyright 2016-2017 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "kicad/dwg.h"
#include "file/file.h"
#include "kicad/lib.h"
#include "kicad/sch.h"


/* ----- Helper functions -------------------------------------------------- */


static const char *sanitize_file_name(const char *name)
{
	const char *slash = strrchr(name, '/');
	const char *colon = strrchr(name, ':');

	if (!slash && !colon)
		return name;
	if (slash && !colon)
		return slash + 1;
	if (!slash && colon)
		return colon + 1;
	if (slash > colon)
		return slash + 1;
	else
		return colon + 1;
}


/* ----- (Global) Labels --------------------------------------------------- */


static enum dwg_shape do_decode_shape(const char *s)
{
	if (!strcmp(s, "UnSpc"))
		return dwg_unspec;
	if (!strcmp(s, "Input"))
		return dwg_in;
	if (!strcmp(s, "Output"))
		return dwg_out;
	if (!strcmp(s, "3State"))
		return dwg_tri;
	if (!strcmp(s, "BiDi"))
		return dwg_bidir;
	fatal("unknown shape: \"%s\"", s);
}


static enum dwg_shape decode_shape(const char *s)
{
	enum dwg_shape res;

	res = do_decode_shape(s);
	free((void *) s);
	return res;
}


static enum text_style decode_style(const char *italic, int bold)
{
	enum text_style res;

	if (!strcmp(italic, "~"))
		res = text_normal;
	else if (!strcmp(italic, "Italic"))
		res = text_italic;
	else {
		error ("unrecognized text attribute \"%s\"", italic);
		res = text_normal;
	}

	if (bold)
		res |= text_bold;

	return res;
}


/* ----- Component fields -------------------------------------------------- */


void decode_alignment(struct text *txt, char hor, char vert)
{
	switch (hor) {
	case 'L':
		txt->hor = text_min;
		break;
	case 'C':
		txt->hor = text_mid;
		break;
	case 'R':
		txt->hor = text_max;
		break;
	default:
		assert(0);
	}

	switch (vert) {
	case 'B':
		txt->vert = text_min;
		break;
	case 'C':
		txt->vert = text_mid;
		break;
	case 'T':
		txt->vert = text_max;
		break;
	default:
		assert(0);
	}
}


static bool parse_field(struct sch_ctx *ctx, const char *line)
{
	struct sch_comp *comp = &ctx->obj.u.comp;
	unsigned flags;
	char hv, hor, vert, italic = 'N', bold = 'N';
	struct comp_field *field;
	struct text *txt;
	char *s;
	const char *p;
	int pos = 0, len;

	field = alloc_type(struct comp_field);
	txt = &field->txt;
	txt->s = NULL;

	/* ignore fields with empty string as content */
	if (sscanf(line, "F %u \"\" %c %d %d %u %u %c %c%c%c",
	    &field->n, &hv, &txt->x, &txt->y, &txt->size, &flags, &hor, &vert,
	    &italic, &bold) >= 8) {
		free(field);
		return 1;
	}

	if (sscanf(line, "F %u \"%n", &field->n, &pos) != 1 || !pos) {
		free(field);
		return 0;
	}
	for (p = line + pos; *p && *p != '"'; p++)
		if (*p == '\\' && p[1])
			p++;
	if (*p != '"') {
		free(field);
		return 0;
	}

	len = p - (line + pos);
	s = alloc_size(len + 1);
	memcpy(s, line + pos, len);
	s[len] = 0;

	if (sscanf(p + 1, " %c %d %d %u %u %c %c%c%c",
	    &hv, &txt->x, &txt->y, &txt->size, &flags,
	    &hor, &vert, &italic, &bold) < 7) {
		free(s);
		free(field);
		return 0;
	}
	txt->s = s;

	field->visible = !flags;

	if (field->n == 0 && comp->comp && comp->comp->units > 1) {
		len = strlen(txt->s);
		s = realloc_size((void *) txt->s, len + 3);
		if (comp->unit <= 26)
			sprintf(s + len, "%c", 'A' + comp->unit - 1);
		else
			sprintf(s + len, "%c%c",
			    'A' + (comp->unit - 1) / 26 - 1,
			    'A' + (comp->unit - 1) % 26);
		txt->s = s;
	}

	field->next = comp->fields;
	comp->fields = field;

	switch (hv) {
	case 'H':
		txt->rot = 0;
		break;
	case 'V':
		txt->rot = 90;
		break;
	default:
		assert(0);
	}

	decode_alignment(txt, hor, vert);

	txt->style = text_normal;
	if (italic == 'I')
		txt->style |= text_italic;
	else if (italic != 'N')
		error("unknown italic value \"%c\"", italic);
	if (bold == 'B')
		txt->style |= text_bold;
	else if (bold != 'N')
		error("unknown bold value \"%c\"", italic);

	return 1;
}


/* ----- Sheet field ------------------------------------------------------- */


static enum dwg_shape decode_form(char form)
{
	switch (form) {
	case 'O':
		return dwg_in;
	case 'I':
		return dwg_out;
	case 'B':
		/* fall through */
	case 'T':
		return dwg_bidir;
	case 'U':
		return dwg_unspec;
	default:
		fatal("unknown form: \"%c\"", form);
	}
}


static int decode_side(char side)
{
	switch (side) {
	case 'L':
		return 2;	/* left */
	case 'B':
		return 1;	/* up */
	case 'R':
		return 0;	/* right */
	case 'T':
		return 3;	/* down */
	default:
		fatal("unknown side: \"%c\"", side);
	}
}


static bool parse_hsheet_field(struct sch_ctx *ctx, const char *line)
{
	struct sch_sheet *sheet = &ctx->obj.u.sheet;
	char *s;
	char form, side;
	unsigned n, dim;
	struct sheet_field *field;

	if (sscanf(line, "F%d \"%m[^\"]\" %u", &n, &s, &dim) == 3) {
		switch (n) {
		case 0:
			sheet->name = s;
			sheet->name_dim = dim;
			return 1;
		case 1:
			sheet->file = s;
			sheet->file_dim = dim;
			return 1;
		default:
			assert(0);
		}
	}

	field = alloc_type(struct sheet_field);
	if (sscanf(line, "F%d \"%m[^\"]\" %c %c %d %d %u",
	    &n, &field->s, &form, &side, &field->x, &field->y, &field->dim)
	    != 7) {
		free(field);
		return 0;
	}
	assert(n >= 2);

	if (side == 'B' || side == 'T') {
		/*
		 * This is beautiful: since there is no indication for rotation
		 * on the hsheet, or the sheet or file fields, we need to look
		 * at whether the imported sheet pins go left or right (no
		 * rotation) or whether they go top or bottom (rotation).
		 *
		 * A sheet with no imported pins lacks these hints, and is
		 * therefore always assumed to be without rotation.
		 *
		 * Eeschema is careful to be consistent, and does not allow
		 * sheets with no imported pins to be rotated. Even better, it
		 * flips rotated sheets where the last imported pin is deleted
		 * back.
		 */
		sheet->rotated = 1;
	}
	field->shape = decode_form(form);
	field->side = decode_side(side);

	field->next = sheet->fields;
	sheet->fields = field;

	return 1;
}


/* ----- Comments ---------------------------------------------------------- */


static void add_comment(struct sheet *sheet, unsigned n, char *s)
{
	unsigned i;

	if (n >= sheet->n_comments) {
		sheet->comments =
		    realloc_type_n(sheet->comments, const char *, n + 1);
		for (i = sheet->n_comments; i <= n; i++)
			sheet->comments[i] = NULL;
		sheet->n_comments = n + 1;
	}
	if (sheet->comments[n]) {
		warning("duplicate comment %d", n);
		free(s);
		return;
	}
	sheet->comments[n] = s;
}


/* ----- Schematics parser ------------------------------------------------- */


static struct sch_obj *submit_obj(struct sch_ctx *ctx, enum sch_obj_type type)
{
	struct sch_obj *obj;

	obj = alloc_type(struct sch_obj);
	*obj = ctx->obj;
	obj->type = type;
	obj->next = NULL;

	*ctx->curr_sheet->next_obj = obj;
	ctx->curr_sheet->next_obj = &obj->next;

	return obj;
}


static struct sheet *new_sheet(struct sch_ctx *ctx)
{
	struct sheet *sheet;

	sheet = alloc_type(struct sheet);
	sheet->title = NULL;
	sheet->file = NULL;
	sheet->mtime = 0;
	sheet->path = NULL;
	sheet->objs = NULL;
	sheet->next_obj = &sheet->objs;
	sheet->next = NULL;

	sheet->size = NULL;
	sheet->w = sheet->h = 0;
	sheet->comments = NULL;
	sheet->n_comments = 0;

	sheet->has_children = 0;

	sheet->oid = NULL;

	ctx->curr_sheet = sheet;

	*ctx->next_sheet = sheet;
	ctx->next_sheet = &sheet->next;

	return sheet;
}


static bool parse_line(const struct file *file, void *user, const char *line);


static struct sheet *recurse_sheet(struct sch_ctx *ctx,
    const struct file *related)
{
	const char *name = ctx->obj.u.sheet.file;
	struct sheet *parent, *sheet;
	struct file file;
	char *tmp;
	void *oid;
	bool res;

	if (!file_open(&file, name, related))
		return NULL;

	parent = ctx->curr_sheet;
	sheet = new_sheet(ctx);
	// should get what file_open really uses @@@
	sheet->file = stralloc(sanitize_file_name(file.name));
	sheet->mtime = file.mtime;
	alloc_printf(&tmp, "%s%s/", parent->path,
	    ctx->obj.u.sheet.name ? ctx->obj.u.sheet.name : "");
	sheet->path = tmp;
	oid = file_oid(&file);
	sheet->oid = oid;

	if (ctx->prev && oid) {
		const struct sheet *other;

		for (other = ctx->prev->sheets; other; other = other->next)
			if (!other->has_children &&
			    file_oid_eq(other->oid, oid)) {
				ctx->curr_sheet = parent;
				sheet->title = stralloc(other->title);
				sheet->objs = other->objs;
				sheet->w = other->w;
				sheet->h = other->h;
				return sheet;
			}
	}

	ctx->state = sch_descr;
	res = file_read(&file, parse_line, ctx);
	file_close(&file);
	if (!res)
		return NULL;	/* leave it to caller to clean up */

	ctx->curr_sheet = parent;
	parent->has_children = 1;

	return sheet;
}


static bool parse_line(const struct file *file, void *user, const char *line)
{
	struct sch_ctx *ctx = user;
	struct sch_obj *obj = &ctx->obj;
	int n = 0;	/* use ONLY for %n */
	int i;
	char *s, *italic;
	int bold;

	switch (ctx->state) {
	case sch_basic:
		if (sscanf(line, "$Comp%n", &n) == 0 && n) {
			ctx->state = sch_comp;
			obj->u.comp.fields = NULL;
			return 1;
		}
		if (sscanf(line, "$Sheet%n", &n) == 0 && n) {
			ctx->state = sch_sheet;
			obj->u.sheet.name = NULL;
			obj->u.sheet.file = NULL;
			obj->u.sheet.rotated = 0;
			obj->u.sheet.fields = NULL;
			obj->u.sheet.sheet = NULL;
			return 1;
		}

		/* Text */

		struct sch_text *text = &obj->u.text;

		if (sscanf(line, "Text Notes %d %d %d %d %ms %d",
		    &obj->x, &obj->y, &text->dir, &text->dim, &italic, &bold)
		    == 6) {
			ctx->state = sch_text;
			obj->u.text.fn = dwg_text;
			obj->u.text.shape = dwg_unspec; /* not used for text */
			obj->u.text.style = decode_style(italic, bold);
			free(italic);
			return 1;
		}
		n = sscanf(line, "Text GLabel %d %d %d %d %ms %ms %d",
		    &obj->x, &obj->y, &text->dir, &text->dim, &s,
		    &italic, &bold);
		if (n == 5 || n == 7) {
			ctx->state = sch_text;
			obj->u.text.fn = dwg_glabel;
			obj->u.text.shape = decode_shape(s);
			if (n == 5) {
				obj->u.text.style = text_normal;
			} else {
				obj->u.text.style = decode_style(italic, bold);
				free(italic);
			}
			return 1;
		}
		if (sscanf(line, "Text HLabel %d %d %d %d %ms %ms %d",
		    &obj->x, &obj->y, &text->dir, &text->dim, &s,
		    &italic, &bold) == 7) {
			ctx->state = sch_text;
			obj->u.text.fn = dwg_hlabel;
			obj->u.text.shape = decode_shape(s);
			obj->u.text.style = decode_style(italic, bold);
			free(italic);
			return 1;
		}
		if (sscanf(line, "Text Label %d %d %d %d %ms %d",
		    &obj->x, &obj->y, &text->dir, &text->dim,
		    &italic, &bold) == 6) {
			ctx->state = sch_text;
			obj->u.text.fn = dwg_label;
			obj->u.text.shape = dwg_unspec;
			    /* not used for (local) labels */
			obj->u.text.style = decode_style(italic, bold);
			free(italic);
			return 1;
		}

		/* Connection */

		if (sscanf(line, "Connection ~ %d %d", &obj->x, &obj->y) == 2) {
			submit_obj(ctx, sch_obj_junction);
			return 1;
		}

		/* NoConn */

		if (sscanf(line, "NoConn ~ %d %d", &obj->x, &obj->y) == 2) {
			submit_obj(ctx, sch_obj_noconn);
			return 1;
		}

		/* Wire */

		if (sscanf(line, "Wire Wire Line%n", &n) == 0 && n) {
			ctx->state = sch_wire;
			obj->u.wire.fn = dwg_wire;
			return 1;
		}
		if (sscanf(line, "Wire Bus Line%n", &n) == 0 && n) {
			ctx->state = sch_wire;
			obj->u.wire.fn = dwg_bus;
			return 1;
		}
		if (sscanf(line, "Wire Notes Line%n", &n) == 0 && n) {
			ctx->state = sch_wire;
			obj->u.wire.fn = dwg_line;
			return 1;
		}

		/* Entry */

		/*
		 * Documentation mentions the following additional variants:
		 *
		 * - Entry Wire Line equivalent:
		 *   Wire Wire Bus
		 *   Entry Wire Bus
		 *
		 * - Entry Bus Bus equivalent:
		 *   Wire Bus Bus
		 */
		if (sscanf(line, "Entry Wire Line%n", &n) == 0 && n) {
			ctx->state = sch_wire;
			obj->u.wire.fn = dwg_wire;
			return 1;
		}
		if (sscanf(line, "Entry Bus Bus%n", &n) == 0 && n) {
			ctx->state = sch_wire;
			obj->u.wire.fn = dwg_bus;
			return 1;
		}

		/* EndSCHEMATC */

		if (sscanf(line, "$EndSCHEMATC%n", &n) == 0 && n) {
			ctx->state = sch_eof;
			return 1;
		}
		break;
	case sch_descr:
		if (sscanf(line, "$Descr %ms %d %d",
		    &ctx->curr_sheet->size,
		    &ctx->curr_sheet->w, &ctx->curr_sheet->h) == 2)
			return 1;

		/*
		 * %[ only accepts non-empty strings, so we fail to parse any
		 * of the following items if the string is empty. However,
		 * since we silently ignore all unrecognized lines, this does
		 * not cause a parse error or warning.
		 */
		if (sscanf(line, "Title \"%m[^\"]\"",
		    &ctx->curr_sheet->title) == 1)
			return 1;
		if (sscanf(line, "Date \"%m[^\"]\"",
		    &ctx->curr_sheet->date) == 1)
			return 1;
		if (sscanf(line, "Rev \"%m[^\"]\"",
		    &ctx->curr_sheet->rev) == 1)
			return 1;
		if (sscanf(line, "Comp \"%m[^\"]\"",
		    &ctx->curr_sheet->comp) == 1)
			return 1;
		if (sscanf(line, "Comment%d \"%m[^\"]\"", &i, &s) == 2) {
			if (i <= 0)
				fatal("%s:%u: invalid comment index %d",
				    file->name, file->lineno, i);
			add_comment(ctx->curr_sheet, i - 1, s);
			return 1;
		}

		if (sscanf(line, "$EndDescr%n", &n) || !n)
			return 1;
		ctx->state = sch_basic;
		return 1;
	case sch_comp:
		if (sscanf(line, "$EndComp%n", &n) == 0 && n) {
			ctx->state = sch_basic;
			submit_obj(ctx, sch_obj_comp);
			return 1;
		}
		if (sscanf(line, "L %ms", &s) == 1) {
			obj->u.comp.comp = lib_find(ctx->lib, s);
			free(s);
			return 1;
		}
		if (sscanf(line, "U %u %u",
		    &obj->u.comp.unit, &obj->u.comp.convert) == 2)
			return 1;
		if (sscanf(line, "P %d %d", &obj->x, &obj->y) == 2)
			return 1;
		if (parse_field(ctx, line))
			return 1;
		if (sscanf(line, "AR %n", &n) == 0 && n)
			return 1; /* @@@ what is "AR" ? */
		n = sscanf(line, " %d %d %d %d",
		    obj->u.comp.m + 1, obj->u.comp.m + 2,
		    obj->u.comp.m + 4, obj->u.comp.m + 5);
		if (n == 3)
			return 1;
		if (n == 4) {
			obj->u.comp.m[0] = obj->x;
			obj->u.comp.m[3] = obj->y;
			return 1;
		}
		break;
	case sch_sheet:
		if (sscanf(line, "$EndSheet%n", &n) == 0 && n) {
			struct sch_obj *sheet_obj;

			sheet_obj = submit_obj(ctx, sch_obj_sheet);
			sheet_obj->u.sheet.error = 0;
			if (ctx->recurse) {
				struct sheet *sheet;

				sheet = recurse_sheet(ctx, file);
				if (!sheet)
					sheet_obj->u.sheet.error = 1;
				if (sheet && sheet_obj->u.sheet.name) {
					free((char *) sheet->title);
					sheet->title =
					    stralloc(sheet_obj->u.sheet.name);
				}
				sheet_obj->u.sheet.sheet = sheet;
			} else {
				sheet_obj->u.sheet.sheet = NULL;
			}
			ctx->state = sch_basic;
			return 1;
		}
		if (sscanf(line, "S %d %d %u %u",
		    &obj->x, &obj->y, &obj->u.sheet.w, &obj->u.sheet.h) == 4)
			return 1;
		if (sscanf(line, "U %*x%n", &n) == 0 && n)
			return 1;
		if (parse_hsheet_field(ctx, line))
			return 1;
		break;
	case sch_text:
		ctx->state = sch_basic;
		{
			const char *from;
			char *to;

			s = alloc_size(strlen(line) + 1);
			from = line;
			to = s;
			while (*from) {
				if (from[0] != '\\' || from[1] != 'n') {
					*to++ = *from++;
					continue;
				}
				*to++ = '\n';
				from += 2;
			}
			*to = 0;
			obj->u.text.s = s;
			submit_obj(ctx, obj->u.text.fn == dwg_glabel ?
			    sch_obj_glabel : sch_obj_text);
		}
		return 1;
	case sch_wire:
		if (sscanf(line, "%d %d %d %d", &obj->x, &obj->y,
		    &obj->u.wire.ex, &obj->u.wire.ey) != 4)
			break;
		submit_obj(ctx, sch_obj_wire);
		ctx->state = sch_basic;
		return 1;
	case sch_eof:
		return 1;
	default:
		BUG("invalid state %d", ctx->state);
	}
	fatal("%s:%u: cannot parse\n\"%s\"", file->name, file->lineno, line);
}


bool sch_parse(struct sch_ctx *ctx, struct file *file, const struct lib *lib,
    const struct sch_ctx *prev)
{
	ctx->curr_sheet->file = stralloc(sanitize_file_name(file->name));
	ctx->curr_sheet->mtime = file->mtime;
	ctx->curr_sheet->path = stralloc("/");
	ctx->lib = lib;
	ctx->prev = prev;
	return file_read(file, parse_line, ctx);
}


void sch_init(struct sch_ctx *ctx, bool recurse)
{
	ctx->state = sch_descr;
	ctx->recurse = recurse;
	ctx->curr_sheet = NULL;
	ctx->sheets = NULL;
	ctx->next_sheet = &ctx->sheets;
	new_sheet(ctx);
}


static void free_comp_fields(struct comp_field *fields)
{
	struct comp_field *next;

	while (fields) {
		next = fields->next;
		free((char *) fields->txt.s);
		free(fields);
		fields = next;
	}
}


static void free_sheet_fields(struct sheet_field *fields)
{
	struct sheet_field *next;

	while (fields) {
		next = fields->next;
		free((char *) fields->s);
		free(fields);
		fields = next;
	}
}


static void free_sheet(struct sheet *sch)
{
	struct sch_obj *obj, *next;

	if (!sch)
		return;
	free((char *) sch->title);
	free((char *) sch->file);
	free((char *) sch->path);
	free((char *) sch->size);
	free(sch->oid);
	for (obj = sch->objs; obj; obj = next) {
		next = obj->next;
		switch (obj->type) {
		case sch_obj_glabel:
		case sch_obj_text:
			free((char *) obj->u.text.s);
			break;
		case sch_obj_comp:
			free_comp_fields(obj->u.comp.fields);
			break;
		case sch_obj_sheet:
			free((char *) obj->u.sheet.name);
			free((char *) obj->u.sheet.file);
			/*
			 * Caller frees all sheets, including this sub-sheet
			 * (obj->u.sheet.sheet), so we don't do this here.
			 */
			free_sheet_fields(obj->u.sheet.fields);
			break;
		default:
			break;
		}
		free(obj);
	}
	free(sch);
}


void sch_free(struct sch_ctx *ctx)
{
	struct sheet *next;

	while (ctx->sheets) {
		next = ctx->sheets->next;
		free_sheet(ctx->sheets);
		ctx->sheets = next;
	}
}
