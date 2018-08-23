/*
 * kicad/sexpr.c - S-expression parser
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "kicad/sexpr.h"


struct stack {
	struct expr **next;
	struct stack *prev;
};

struct sexpr_ctx {
	enum sexpr_state {
		idle,
		token,
		string,
		escape,
		failed
	} state;
	struct expr *e;		/* expression */
	const char *p;		/* beginning of string */
	struct stack *sp;
	struct stack stack;
};


/* ----- Parser ------------------------------------------------------------ */


static bool pop(struct sexpr_ctx *ctx)
{
	struct stack *prev;

	if (!ctx->sp->prev) {
		error("too many )");
		return 0;
	}
	prev = ctx->sp->prev;
	free(ctx->sp);
	ctx->sp = prev;
	return 1;
}


static void new_expr(struct sexpr_ctx *ctx)
{
	struct stack *st = alloc_type(struct stack);
	struct expr *e = alloc_type(struct expr);

	e->s = NULL;
	e->e = NULL;
	e->next = NULL;

	*ctx->sp->next = e;
	ctx->sp->next = &e->next;

	st->prev = ctx->sp;
	st->next = &e->e;
	ctx->sp = st;
}


static void add_string(struct sexpr_ctx *ctx, const char *end)
{
	struct expr *e;
	unsigned old;
	unsigned new = end - ctx->p;

	e = *ctx->sp->next;
	if (e) {
		old = strlen(e->s);
	} else {
		e = alloc_type(struct expr);
		e->s = NULL;
		e->e = NULL;
		e->next = NULL;

		*ctx->sp->next = e;
		old = 0;
	}

	if (!new)
		return;

	e->s = realloc_size(e->s, old + new + 1);
	memcpy(e->s + old, ctx->p, new);
	e->s[old + new] = 0;
}


static void end_string(struct sexpr_ctx *ctx, const char *end)
{
	struct expr *e;
	char *s, *t;

	add_string(ctx, end);
	e = *ctx->sp->next;
	ctx->sp->next = &e->next;

	for (s = t = e->s; *s; s++) {
		if (*s == '\\')
			switch (*++s) {
			case 'n':
				*t++ = '\n';
				continue;
			case 't':
				*t++ = '\t';
				continue;
			case 0:
				BUG("escaped NUL");
			default:
				break;
			}
		*t++ = *s;
	}
	*t = 0;
}


bool sexpr_parse(struct sexpr_ctx *ctx, const char *s)
{
	ctx->p = s;
	while (*s) {
		switch (ctx->state) {
		case idle:
			switch (*s) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				break;
			case '(':
				new_expr(ctx);
				break;
			case ')':
				if (!pop(ctx))
					ctx->state = failed;
				break;
			case '"':
				ctx->state = string;
				ctx->p = s + 1;
				break;
			default:
				ctx->p = s;
				ctx->state = token;
				break;
			}
			break;
		case token:
			switch (*s) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				ctx->state = idle;
				end_string(ctx, s);
				break;
			case '(':
			case ')':
			case '"':
				ctx->state = idle;
				end_string(ctx, s);
				continue;
			default:
				break;
			}
			break;
		case string:
			switch (*s) {
			case '\r':
			case '\n':
				ctx->state = failed;
				error("newline in string");
				break;
			case '"':
				ctx->state = idle;
				end_string(ctx, s);
				break;
			case '\\':
				ctx->state = escape;
				break;
			default:
				break;
			}
			break;
		case escape:
			switch (*s) {
			case '\r':
			case '\n':
				ctx->state = failed;
				error("newline in string");
				break;
			default:
				ctx->state = string;
				break;
			}
			break;
		case failed:
			return 0;
		default:
			BUG("invalid state %d", ctx->state);
		}
		s++;
	}

	switch (ctx->state) {
	case token:
	case string:
	case escape:
		add_string(ctx, s);
		break;
	default:
		break;
	}

	return 1;
}


/* ----- Dumping ----------------------------------------------------------- */


static void do_dump_expr(const struct expr *e, unsigned depth)
{
	bool need_nl = 0;

	if (depth)
		printf("%*s(", (depth - 1) * 2, "");
	while (e) {
		if (e->s) {
			if (need_nl) {
				printf("\n%*s", depth * 2, "");
				need_nl = 0;
			}
			printf("\"%s\" ", e->s); /* @@@ escaping */
		}
		if (e->e) {
			putchar('\n');
			do_dump_expr(e->e, depth + 1);
			need_nl = 1;
		}
		e = e->next;
	}
	if (depth)
		putchar(')');
}


void dump_expr(const struct expr *e)
{
	do_dump_expr(e, 0);
	putchar('\n');
}


/* ----- Cleanup ----------------------------------------------------------- */


static void free_stack(struct sexpr_ctx *ctx)
{
	struct stack *prev;

	while (ctx->sp != &ctx->stack) {
		prev = ctx->sp->prev;
		free(ctx->sp);
		ctx->sp = prev;
	}
}


void free_expr(struct expr *e)
{
	struct expr *next;

	while (e) {
		next = e->next;
		if (e->s)
			free(e->s);
		else if (e->e)
			free_expr(e->e);
		free(e);
		e = next;
	}
}


/* ----- Parser creation --------------------------------------------------- */


struct sexpr_ctx *sexpr_new(void)
{
	struct sexpr_ctx *ctx;

	ctx = alloc_type(struct sexpr_ctx);
	ctx->state = idle;
	ctx->e = NULL;
	ctx->p = NULL;
	ctx->sp = &ctx->stack;
	ctx->stack.next = &ctx->e;
	ctx->stack.prev = NULL;
	return ctx;
}


/* ----- Termination ------------------------------------------------------- */


void sexpr_abort(struct sexpr_ctx *ctx)
{
	free_stack(ctx);
	free_expr(ctx->e);
	free(ctx);
}


bool sexpr_finish(struct sexpr_ctx *ctx, struct expr **res)
{
	if (ctx->sp != &ctx->stack) {
		error("not enough )");
		ctx->state = failed;
	}
	if (ctx->state != idle && ctx->state != failed)
		error("invalid end state %d", ctx->state);
	if (ctx->state != idle) {
		sexpr_abort(ctx);
		return 0;
	}
	if (res)
		*res = ctx->e;
	else
		free_expr(ctx->e);
	free(ctx);
	return 1;
}
