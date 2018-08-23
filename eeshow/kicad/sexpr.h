/*
 * kicad/sexpr.h - S-expression parser
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_SEXPR_H
#define	KICAD_SEXPR_H

#include <stdbool.h>


/*
 * Expression type:
 *
 * s		e		Meaning
 * ------------	--------	----------
 * NULL		NULL		"" or ()
 * non-NULL	NULL		foo or "foo"
 * NULL		non-NULL	(...)
 * non-NULL	non-NULL	trouble
 */

struct expr {
	char *s;		/* string or NULL */
	struct expr *e;		/* sub-sexpr or NULL*/
	struct expr *next;
};

struct sexpr_ctx;


void dump_expr(const struct expr *e);
void free_expr(struct expr *e);

struct sexpr_ctx *sexpr_new(void);
bool sexpr_parse(struct sexpr_ctx *ctx, const char *s);
void sexpr_abort(struct sexpr_ctx *ctx);
bool sexpr_finish(struct sexpr_ctx *ctx, struct expr **res);

#endif /* !KICAD_SEXPR_H */
