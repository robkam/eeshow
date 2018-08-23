/*
 * kicad/delta.c - Find differences in .sch files
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/text.h"
#include "kicad/lib.h"
#include "kicad/sch.h"
#include "kicad/delta.h"


/*
 * @@@ we should clone components and compare them, too
 *
 * if two components or sub-sheets mismatch in their field lists, we should
 * remove matching items from the lists.
 *
 * @@@@ line(A, B) == line(B, A), etc.
 */


/* ----- Components -------------------------------------------------------- */


static bool comp_eq_obj(const struct lib_obj *a, const struct lib_obj *b)
{
	int i;

	if (a->type != b->type)
		return 0;
	if (a->unit != b->unit || a->convert != b->convert)
			return 0;
	switch (a->type) {
	case lib_obj_poly:
		if (a->u.poly.thick != b->u.poly.thick &&
		    a->u.poly.fill != b->u.poly.fill)
			return 0;
		if (a->u.poly.points != b->u.poly.points)
			return 0;
		for (i = 0; i != a->u.poly.points; i++)
			if (a->u.poly.x[i] != b->u.poly.x[i] ||
			    a->u.poly.y[i] != b->u.poly.y[i])
				return 0;
		return 1;
	case lib_obj_rect:
		return a->u.rect.sx == b->u.rect.sx &&
		    a->u.rect.sy == b->u.rect.sy &&
		    a->u.rect.ex == b->u.rect.ex &&
		    a->u.rect.ey == b->u.rect.ey &&
		    a->u.rect.thick == b->u.rect.thick &&
		    a->u.rect.fill == b->u.rect.fill;
	case lib_obj_circ:
		return a->u.circ.x == b->u.circ.x &&
		    a->u.circ.y == b->u.circ.y &&
		    a->u.circ.r == b->u.circ.r &&
		    a->u.circ.thick == b->u.circ.thick &&
		    a->u.circ.fill == b->u.circ.fill;
	case lib_obj_arc:
		return a->u.arc.x == b->u.arc.x &&
		    a->u.arc.y == b->u.arc.y &&
		    a->u.arc.r == b->u.arc.r &&
		    a->u.arc.start_a == b->u.arc.start_a &&
		    a->u.arc.end_a == b->u.arc.end_a &&
		    a->u.arc.thick == b->u.arc.thick &&
		    a->u.arc.fill == b->u.arc.fill;
	case lib_obj_text:
		return a->u.text.x == b->u.text.x &&
		    a->u.text.y == b->u.text.y &&
		    a->u.text.dim == b->u.text.dim &&
		    a->u.text.orient == b->u.text.orient &&
		    a->u.text.style == b->u.text.style &&
		    a->u.text.hor_align == b->u.text.hor_align &&
		    a->u.text.vert_align == b->u.text.vert_align &&
		    !strcmp(a->u.text.s, b->u.text.s);
	case lib_obj_pin:
		return a->u.pin.x == b->u.pin.x &&
		    a->u.pin.y == b->u.pin.y &&
		    a->u.pin.length == b->u.pin.length &&
		    a->u.pin.orient == b->u.pin.orient &&
		    a->u.pin.number_size == b->u.pin.number_size &&
		    a->u.pin.name_size == b->u.pin.name_size &&
		    a->u.pin.etype == b->u.pin.etype &&
		    a->u.pin.shape == b->u.pin.shape &&
		    !strcmp(a->u.pin.name, b->u.pin.name) &&
		    !strcmp(a->u.pin.number, b->u.pin.number);
	default:
		BUG("invalid type %d", a->type);
	}
}


static bool comp_eq_objs(const struct lib_obj *a, const struct lib_obj *b)
{
	/*
	 * @@@ over-simplify a little. We don't search to find objects that
	 * have merely been reordered.
	 */
	while (a && b) {
		if (!comp_eq_obj(a, b))
			return 0;
		a = a->next;
		b = b->next;
	}
	return a == b;
}


static bool comp_eq_aliases(const struct comp_alias *a,
    const struct comp_alias *b)
{
	while (a && b) {
		if (strcmp(a->name, b->name))
			return 0;
		a = a->next;
		b = b->next;
	}
	return a == b;
}


static bool comp_eq(const struct comp *a, const struct comp *b)
{
	if (a == b)
		return 1;
	if (!(a && b))
		return 0;
	if (strcmp(a->name, b->name))
		return 0;
	if (a->units != b->units)
		return 0;
#if 0
	/* @@@ in-sheet settings override this */
	if (a->visible != b->visible)
		return 0;
#endif
	if (a->show_pin_name != b->show_pin_name)
		return 0;
	if (a->show_pin_num != b->show_pin_num)
		return 0;
	if (a->name_offset != b->name_offset)
		return 0;
	if (!comp_eq_aliases(a->aliases, b->aliases))
		return 0;
	return comp_eq_objs(a->objs, b->objs);
}


/* ----- Sheets ------------------------------------------------------------ */


static struct sch_obj *objs_clone(const struct sch_obj *objs)
{
	struct sch_obj *new_objs = NULL;
	struct sch_obj **next = &new_objs;
	const struct sch_obj *obj;

	for (obj = objs; obj; obj = obj->next) {
		*next = alloc_type(struct sch_obj);
		**next = *obj;
		next = &(*next)->next;
	}
	*next = NULL;
	return new_objs;
}


static void add_obj(struct sheet *sch, struct sch_obj *obj)
{
	*sch->next_obj = obj;
	sch->next_obj = &obj->next;
	obj->next = NULL;
}


static bool comp_fields_eq(const struct comp_field *a,
    const struct comp_field *b)
{
	while (a && b) {
		if (a->txt.x != b->txt.x || a->txt.y != b->txt.y)
			return 0;
		if (a->txt.size != b->txt.size)
			return 0;
		if (a->txt.rot != b->txt.rot)
			return 0;
		if (a->txt.hor != b->txt.hor || a->txt.vert != b->txt.vert)
			return 0;
		if (strcmp(a->txt.s, b->txt.s))
			return 0;
		a = a->next;
		b = b->next;
	}
	return a == b;
}


static bool sheet_fields_eq(const struct sheet_field *a,
    const struct sheet_field *b)
{
	const struct sheet_field *ta;
	const struct sheet_field *tb;

	for (ta = a; ta; ta = ta->next) {
		for (tb = b; tb; tb = tb->next) {
			if (ta->x != tb->x || ta->y != tb->y)
				continue;
			if (ta->dim != tb->dim)
				continue;
			if (ta->dim != tb->dim)
				continue;
			if (ta->shape != tb->shape)
				continue;
			if (strcmp(ta->s, tb->s))
				continue;
			goto match;
		}
		return 0;
match:
		continue;
	}

	while (a && b) {
		a = a->next;
		b = b->next;
	}
	return a == b;
}


/*
 * @@@ idea: if we send all strings through a "unique" function, we can
 * memcmp things like "struct sch_text" without having to go through the
 * fields individually.
 */

static bool obj_eq(const struct sch_obj *a, const struct sch_obj *b,
    bool recurse)
{
	if (a->type != b->type)
		return 0;

	/* Special treatment because we support direction reversal */
	if (a->type == sch_obj_wire) {
		if (a->u.wire.fn != b->u.wire.fn)
			return 0;
		if (a->x == b->x && a->y == b->y &&
		    a->u.wire.ex == b->u.wire.ex &&
		    a->u.wire.ey == b->u.wire.ey)
			return 1;
		if (a->x == b->u.wire.ex && a->y == b->u.wire.ey &&
		    a->u.wire.ex == b->x && a->u.wire.ey == b->y)
			return 1;
		return 0;
	}

	if (a->x != b->x || a->y != b->y)
		return 0;

	switch (a->type) {
	case sch_obj_junction:
		return 1;
	case sch_obj_noconn:
		return 1;
	case sch_obj_glabel:
	case sch_obj_text:
		if (a->u.text.fn != b->u.text.fn)
			return 0;
		if (a->u.text.dir != b->u.text.dir)
			return 0;
		if (a->u.text.dim != b->u.text.dim)
			return 0;
		if (a->u.text.shape != b->u.text.shape)
			return 0;
		if (strcmp(a->u.text.s, b->u.text.s))
			return 0;
		return 1;
	case sch_obj_comp:
		if (!comp_eq(a->u.comp.comp, b->u.comp.comp))
			return 0;
		if (a->u.comp.unit != b->u.comp.unit)
			return 0;
		if (a->u.comp.convert != b->u.comp.convert)
			return 0;
		if (memcmp(a->u.comp.m, b->u.comp.m, sizeof(a->u.comp.m)))
			return 0;
		return comp_fields_eq(a->u.comp.fields, b->u.comp.fields);
	case sch_obj_sheet:
		if (a->u.sheet.w != b->u.sheet.w)
			return 0;
		if (a->u.sheet.h != b->u.sheet.h)
			return 0;
		if (a->u.sheet.name_dim != b->u.sheet.name_dim)
			return 0;
		if (a->u.sheet.file_dim != b->u.sheet.file_dim)
			return 0;
		if (a->u.sheet.rotated != b->u.sheet.rotated)
			return 0;
		if (strcmp(a->u.sheet.name, b->u.sheet.name))
			return 0;
		if (strcmp(a->u.sheet.file, b->u.sheet.file))
			return 0;
		if (!sheet_fields_eq(a->u.sheet.fields, b->u.sheet.fields))
			return 0;
		if (!recurse)
			return 1;
		if (a->u.sheet.error != b->u.sheet.error)
			return 0;
		return sheet_eq(a->u.sheet.sheet, b->u.sheet.sheet, 1);
	default:
		BUG("invalid type %d", a->type);
	}
}


bool sheet_eq(const struct sheet *a, const struct sheet *b, bool recurse)
{
	const struct sch_obj *obj_a, *obj_b;

	if (a == NULL && b == NULL)
		return 1;
	if (!(a && b))
		return 0;

	if (a->title != b->title) {
		if (!a->title || !b->title)
			return 0;
		if (strcmp(a->title, b->title))
			return 0;
	}

	obj_a = a->objs;
	obj_b = b->objs;
	while (obj_a && obj_b) {
		if (!obj_eq(obj_a, obj_b, recurse))
			return 0;
		obj_a = obj_a->next;
		obj_b = obj_b->next;
	}
	return obj_a == obj_b;
}


/* ----- Merge wires ------------------------------------------------------- */


static int min(int a, int b)
{
	return a < b ? a : b;
}


static int max(int a, int b)
{
	return a > b ? a : b;
}


static void merge_coord(int *sa, int *ea, int sb, int eb)
{
	int tmp;

	tmp = min(min(*sa, *ea), min(sb, eb));
	*ea = max(max(*sa, *ea), max(sb, eb));
	*sa = tmp;
}


static bool merge_wire(struct sch_obj *a, const struct sch_obj *b)
{
	struct sch_wire *wa = &a->u.wire;
	const struct sch_wire *wb = &b->u.wire;

	if (a->x == b->x && a->x == wa->ex && a->x == wb->ex &&
	    (a->y == b->y || a->y == wb->ey || wa->ey == b->y ||
	    wa->ey == wb->ey)) {
		merge_coord(&a->y, &wa->ey, b->y, wb->ey);
		return 1;
	}
	if (a->y == b->y && a->y == wa->ey && a->y == wb->ey &&
	    (a->x == b->x || a->x == wb->ex || wa->ex == b->x ||
	    wa->ex == wb->ex)) {
		merge_coord(&a->x, &wa->ex, b->x, wb->ex);
		return 1;
	}
	return 0;
}


static void merge_wires(struct sch_obj *a)
{
	struct sch_obj **b, **next;

	for (; a; a = a->next) {
		if (a->type != sch_obj_wire)
			continue;
		for (b = &a->next; *b; b = next) {
			next = &(*b)->next;
			if ((*b)->type != sch_obj_wire)
				continue;
			if (a->u.wire.fn != (*b)->u.wire.fn)
				continue;
			if (merge_wire(a, *b)) {
				struct sch_obj *tmp = *b;

				*b = *next;
				free(tmp);
				next = b;
			}
		}
	}
}


/* ----- Split objects from A and B into only-A, only-B, and A-and-B ------- */


static void free_obj(struct sch_obj *obj)
{
	/* there may be more to free once we get into cloning components */
	free(obj);
}


static void init_res(struct sheet *res)
{
	res->title = NULL;
	res->file = NULL;
	res->path = NULL;
	res->objs = NULL;
	res->next_obj = &res->objs;
	res->next = NULL;
}


void delta(const struct sheet *a, const struct sheet *b,
    struct sheet *res_a, struct sheet *res_b, struct sheet *res_ab)
{
	struct sch_obj *objs_a, *objs_b;
	struct sch_obj *next_a;
	struct sch_obj **obj_b;

	init_res(res_a);
	init_res(res_b);
	init_res(res_ab);

	if (a->title && b->title && !strcmp(a->title, b->title)) {
		res_ab->title = a->title;
	} else {
		res_a->title = a->title;
		res_b->title = b->title;
	}

	if (a->file && b->file && !strcmp(a->file, b->file)) {
		res_ab->file = a->file;
	} else {
		res_a->file = a->file;
		res_b->file = b->file;
	}

	if (a->path && b->path && !strcmp(a->path, b->path)) {
		res_ab->path = a->path;
	} else {
		res_a->path = a->path;
		res_b->path = b->path;
	}

	objs_a = objs_clone(a->objs);
	objs_b = objs_clone(b->objs);

	merge_wires(objs_a);
	merge_wires(objs_b);

	while (objs_a) {
		next_a = objs_a->next;
		for (obj_b = &objs_b; *obj_b; obj_b = &(*obj_b)->next)
			if (obj_eq(objs_a, *obj_b, 0)) {
				struct sch_obj *tmp = *obj_b;

				add_obj(res_ab, objs_a);
				*obj_b = tmp->next;
				free_obj(tmp);
				goto found;
			}
		add_obj(res_a, objs_a);
found:
		objs_a = next_a;
	}
	res_b->objs = objs_b;
}


void delta_free(struct sheet *d)
{
	struct sch_obj *next;

	while (d->objs) {
		next = d->objs->next;
		free_obj(d->objs);
		d->objs = next;
	}
}
