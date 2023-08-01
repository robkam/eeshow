/*
 * kicad/pl-render.c - RenderKiCad page layout
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define	_XOPEN_SOURCE		/* for strptime */
#define	_XOPEN_SOURCE_EXTENDED	/* to not lose strdup */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "version.h"
#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/style.h"
#include "gfx/text.h"
#include "gfx/gfx.h"
#include "kicad/sch.h"
#include "kicad/pl-common.h"
#include "kicad/pl.h"


/*
 * For uses where we take libraries from a .pro but don't want the page layout,
 * too. This is especially important in non-interactive diff mode.
 */

bool suppress_page_layout = 1;

const char *date_override = NULL;

#define DEFAULT_DATE_FORMAT	"%F %T %z"
#define	KICAD_DATE_FORMAT	"%F"


/* ----- Coordinate transform ---------------------------------------------- */


/*
 * Eeschema works in mil
 * Page layouts are in mm
 */


static int mil(float mm)
{
	return mm / 25.4 * 1000;
}


static int coord(int v, int d, int o, int e)
{
	return d >= 0 ? o + v : e - v;
}


/* ----- Date formatting --------------------------------------------------- */


static char *format_date(const char *fmt, const char *sheet_date,
    time_t mtime, time_t max_mtime)
{
	char *s = stralloc("");
	time_t user_time = -1;
	const char *p = fmt;
	char *arg = NULL;
	struct tm tm;
	const char *end;
	time_t t;

	while (*p) {
		int len;

		free(arg);
		arg = NULL;

		len = strlen(s);

		if (p[0] != '%' || p[1] == '%') {
			if (*p == '%')
				p++;
			s = realloc_size(s, len + 2);
			s[len] = *p;
			s[len + 1] = 0;
			p++;
			continue;
		}
		if (*++p == '{') {
			char *to;

			/* allocate worst-case length */
			arg = to = alloc_size(strlen(p) + 1);

			for (p++; *p && *p != '}'; *to++ = *p++)
				if (p[0] == '%' && p[1] == '}')
					p++;
			if (!*p) {
				error("incomplete argument in \"%s\"", fmt);
				free(arg);
				return s;
			}
			*to = 0;
			p++;
		}
		if (*p == '!') {
			p++;
			if (user_time != -1) {
				p++;
				continue;
			}
		}
		switch (*p++) {
		case 'i':	/* parse sheet_date */
			memset(&tm, 0, sizeof(tm));
			end = strptime(sheet_date,
			    arg ? arg : KICAD_DATE_FORMAT, &tm);
			if (end && !*end)
			    user_time = mktime(&tm);
			else
				user_time = -1;
			continue;
		case 's':
			t = mtime;
			break;
		case 'S':
			t = max_mtime;
			break;
		case 't':	/* local time */
			time(&t);
			break;
		case 'u':	/* user time */
			if (user_time == -1)
				continue;
			t = user_time;
			break;
		case 0:
		default:
			error("unrecognized date \"%c\"", p[-1]);
			continue;
		}

		char buf[1000]; /* @@@ enough :) */
		size_t size;

		if (t == -1)
			continue;
		if (!localtime_r(&t, &tm))
			continue;
		size = strftime(buf, sizeof(buf),
		    arg ? arg : DEFAULT_DATE_FORMAT, &tm);
		s = realloc_size(s, len + size + 1);
		memcpy(s + len, buf, size);
		s[len + size] = 0;
	}

	free(arg);

	return s;
}


/* ----- String expansion -------------------------------------------------- */


static time_t max_mtime(const struct sheet *sheets)
{
	time_t t = 0;

	while (sheets) {
		if (sheets->mtime > t)
			t = sheets->mtime;
		sheets = sheets->next;
	}
	return t;
}


static char *expand(const struct pl_ctx *pl, const char *s,
    const struct sheet *sheets, const struct sheet *sheet)
{
	const struct sheet *sch;
	char *res = NULL;
	const char *p;
	unsigned size = 0;
	unsigned len;
	char *x;
	const char *cx;
	unsigned n;

	while (1) {
		cx = x = NULL;
		p = strchr(s, '%');
		if (!p)
			break;
		switch (p[1]) {
		case '%':
			cx = "%";
			break;
		case 'C':		// comment #n
			if (isdigit(p[2])) {
				n = p[2] - '0';
				if (n >= sheet->n_comments)
					cx = "";
				else
					cx = sheet->comments[n];
			} else {
				warning("%%C without number");
				cx = "???";
			}
			break;
		case 'D':		// date
			if (date_override)
				x = format_date(date_override,
				    sheet->date ? sheet->date : "",
				    sheet->mtime, max_mtime(sheets));
			else
				cx = sheet->date;
			break;
		case 'F':		// file name
			cx = sheet->file;
			break;
		case 'K':		// KiCad version
			alloc_printf(&x, "Plotted by eeshow %s %s",
			    version, build_date);
			break;
		case 'N':		// number of sheets
			n = 0;
			for (sch = sheets; sch; sch = sch->next)
				n++;
			alloc_printf(&x, "%u", n);
			break;
		case 'P':		// sheet path
			cx = sheet->path;
			break;
		case 'R':		// revision
			cx = sheet->rev;
			break;
		case 'S':		// sheet number
			n = 1;
			for (sch = sheets; sch != sheet;
			    sch = sch->next)
				n++;
			alloc_printf(&x, "%u", n);
			break;
		case 'T':		// title
			cx = sheet->title;
			break;
		case 'Y':		// company name
			cx = sheet->comp;
			break;
		case 'Z':		// paper format
			cx = sheet->size;
			break;
		default:
			cx = "???";
			break;
		}
		if (!cx)
			cx = x ? x : "";
		len = strlen(cx);
		res = realloc_size(res, size + p - s + len);
		memcpy(res + size, s, p - s);
		size += p - s;
		s = p[1] == 'C' && p[2] ? p + 3 : p + 2;
		memcpy(res + size, cx, len);
		size += len;
		free(x);
	}

	len = strlen(s);
	res = realloc_size(res, size + len + 1);
	memcpy(res + size, s, len + 1);
	return res;
}


/* ----- Rendering --------------------------------------------------------- */


static char *increment(char *s, int inc, const char *range)
{
	char *t;
	unsigned len = strlen(s);
	int base, n;

	t = realloc_size(s, len + 2);
	t[len + 1] = 0;

	base = range[1] - range[0] + 1;
	n = t[len - 1] - range[0] + inc;
	t[len - 1] = n / base + range[0];
	t[len] = n % base + range[0];
	return t;
}


static void render_text(const struct pl_ctx *pl, const struct pl_obj *obj,
    struct gfx *gfx, int x, int y, int inc,
    const struct sheet *sheets, const struct sheet *sheet)
{
	char *s = expand(pl, obj->s, sheets, sheet);
	struct text txt = {
		.size	= mil(obj->ey ? obj->ey : pl->ty),
		.x	= x,
		.y	= y,
		.rot	= obj->rotate,
		.hor	= obj->hor,
		.vert	= obj->vert,
		.style	= text_normal,	// @@@
	};

	if (inc && *s) {
		char *end = strchr(s, 0) - 1;
		const char *range = NULL;

		if (isdigit(*end))
			range = "09";
		else if (isupper(*end))
			range = "AZ";
		else if (islower(*end))
			range = "az";
		if (range) {
			 if (*end + inc <= range[1])
				*end += inc;
			else
				s = increment(s, inc, range);
		}
	}
	txt.s = s;
	text_show(&txt, gfx, COLOR_COMP_DWG, LAYER_COMP_DWG);
	free(s);
}


static void render_poly(const struct pl_obj *obj, const struct pl_poly *poly,
    struct gfx *gfx, int x, int y)
{
	double a = obj->rotate / 180.0 * M_PI;
	const struct pl_point *p;
	unsigned n = 0;
	int px, py;

	for (p = poly->points; p; p = p->next)
		n++;

	int vx[n];
	int vy[n];

	n = 0;
	for (p = poly->points; p; p = p->next) {
		px = mil(p->x);
		py = mil(p->y);
		vx[n] = x + cos(a) * px + sin(a) * py;
		vy[n] = y + cos(a) * py - sin(a) * px;
		n++;
	}
	gfx_poly(gfx, n, vx, vy,
	    COLOR_COMP_DWG, COLOR_COMP_DWG, LAYER_COMP_DWG);
}


static void render_polys(const struct pl_obj *obj, struct gfx *gfx,
    int x, int y)
{
	const struct pl_poly *poly;

	for (poly = obj->poly; poly; poly = poly->next)
		render_poly(obj, poly, gfx, x, y);
}


static void render_obj(const struct pl_ctx *pl, const struct pl_obj *obj,
    struct gfx *gfx, unsigned inc,
    const struct sheet *sheets, const struct sheet *sheet)
{
	int w = sheet->w;
	int h = sheet->h;
	int xo = mil(pl->l);
	int yo = mil(pl->r);
	int xe = w - mil(pl->t);
	int ye = h - mil(pl->b);
	int x = mil(obj->x + inc * obj->incrx);
	int y = mil(obj->y + inc * obj->incry);
	int ex = mil(obj->ex + inc * obj->incrx);
	int ey = mil(obj->ey + inc * obj->incry);
	int ww = xe - xo;
	int hh = ye - yo;

	if (x < 0 || y < 0 || ex < 0 || ey < 0)
		return;
	if (x > ww || y > hh || ex > ww || ey > hh)
		return;

	x = coord(x, obj->dx, xo, xe);
	y = coord(y, obj->dy, yo, ye);
	ex = coord(ex, obj->edx, xo, xe);
	ey = coord(ey, obj->edy, yo, ye);

	switch (obj->type) {
	case pl_obj_rect:
		gfx_rect(gfx, x, y, ex, ey,
		    COLOR_COMP_DWG, COLOR_NONE, LAYER_COMP_DWG);
		break;
	case pl_obj_line: {
			int vx[] = { x, ex };
			int vy[] = { y, ey };

			gfx_poly(gfx, 2, vx, vy,
			    COLOR_COMP_DWG, COLOR_NONE, LAYER_COMP_DWG);
		}
		break;
	case pl_obj_text:
		render_text(pl, obj, gfx, x, y, inc, sheets, sheet);
		break;
	case pl_obj_poly:
		render_polys(obj, gfx, x, y);
		break;
	default:
		break;
	}
}


void pl_render(struct pl_ctx *pl, struct gfx *gfx, const struct sheet *sheets,
    const struct sheet *sheet)
{
	const struct pl_obj *obj;
	int i;

	if (suppress_page_layout)
		return;
	for (obj = pl->objs; obj; obj = obj->next)
		for (i = 0; i != obj->repeat; i++)
			if (obj->pc == pc_none ||
			    (obj->pc == pc_only_one) == (sheets == sheet))
				render_obj(pl, obj, gfx, i, sheets, sheet);
}
