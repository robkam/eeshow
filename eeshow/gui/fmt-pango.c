/*
 * gui/fmt-pango.c - Format strings for Pango markup
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <alloca.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gui/fmt-pango.h"


unsigned vsfmt_pango(char *buf, const char *fmt, va_list ap)
{
	char *res;
	const char *p, *q, *s, *t;
	char *u;
	char *tmp_fmt;
	char *tmp, *tmp2;
	int len;
	unsigned extra;

	res = buf;
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			if (buf)
				*res = *p;
			res++;
			continue;
		}
		for (q = p + 1; isdigit(*q) || *q == '.' || *q == '-'; q++);
		tmp_fmt = alloca(q - p + 1 + 1);
		memcpy(tmp_fmt, p, q - p + 1);
		tmp_fmt[q - p + 1] = 0;
		switch (*q) {
		case 's':
			s = va_arg(ap, const char *);
			len = alloc_printf(&tmp, tmp_fmt, s);

			extra = 0;
			for (t = tmp; *t; t++)
				switch (*t) {
				case '<':
				case '>':
					extra += 3;
					break;
				case '&':
					extra += 4;
					break;
				default:
					break;
				}

			if (extra) {
				tmp2 = u = alloca(len + extra + 1);
				for (t = tmp; *t; t++) {
					switch (*t) {
					case '<':
						strcpy(u, "&lt;");
						u += 4;
						break;
					case '>':
						strcpy(u, "&gt;");
						u += 4;
						break;
					case '&':
						strcpy(u, "&amp;");
						u += 5;
						break;
					default:
						*u++ = *t;
						break;
					}
				}
				*u = 0;
				tmp = tmp2;
			}

			if (buf)
				memcpy(res, tmp, len + extra);
			res += len + extra;
			break;
		case 'c':
			/* @@@ we don't filter markup meta-characters */
		case 'd':
		case 'x':
			len = alloc_printf(&tmp, tmp_fmt, va_arg(ap, int));
			if (buf)
				memcpy(res, tmp, len);
			res += len;
			break;
		case 'u':
			len = alloc_printf(&tmp, tmp_fmt, va_arg(ap, unsigned));
			if (buf)
				memcpy(res, tmp, len);
			res += len;
			break;
		case '%':
			if (buf)
				*res = '%';
			res++;
			break;
		default:
			fatal("unrecognized format '%%%c'\n", *q);
		}
		p = q;
	}
	if (buf)
		*res = 0;
	return res - buf;
}


char *vfmt_pango(const char *fmt, va_list ap)
{
	va_list aq;
	unsigned len;
	char *buf;

	va_copy(aq, ap);
	len = vsfmt_pango(NULL, fmt, ap);
	buf = alloc_size(len + 1);
	vsfmt_pango(buf, fmt, aq);
	va_end(aq);
	return buf;
}


char *fmt_pango(const char *fmt, ...)
{
	va_list ap;
	char *buf;

	va_start(ap, fmt);
	buf = vfmt_pango(fmt, ap);
	va_end(ap);
	return buf;
}
