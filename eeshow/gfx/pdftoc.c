/*
 * gfx/pdftoc.c - PDF writer with TOC generation
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Strongly influenced by https://neo900.org/git/misc/tree/schtoc
 *
 * PDF Reference:
 * http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 */


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/pdftoc.h"


struct title {
	char *s;
	struct title *next;
};

struct object {
	int gen;
	unsigned pos;
	bool is_page;
};

struct pdftoc {
	FILE *file;	/* NULL if not set / stdout */

	enum state {
		idle,	/* between objects */
		object,	/* inside an object */
		catalog,/* inside the catalog object */
		xref,	/* stopped at xref */
		trailer,/* going through the trailer */
	} state;

	struct title *titles;
	struct title **next_title;
	unsigned n_titles;

	char *buf;
	unsigned left;		/* bytes left in buffer */
	unsigned offset;	/* offset into buffer */
	unsigned pos;		/* position in file */

	struct object *objs;	/* object array */
	struct object *curr_obj;
	int top;		/* highest object number; -1 if no objects */

	int root;		/* catalog dict */
	int info;		/* information dict, 0 if absent */
};


struct pdftoc *pdftoc_begin(void)
{
	struct pdftoc *ctx;

	ctx = alloc_type(struct pdftoc);
	ctx->file = NULL;
	ctx->state = idle;

	ctx->titles = NULL;
	ctx->next_title = &ctx->titles;
	ctx->n_titles = 0;

	ctx->buf = NULL;
	ctx->left = 0;
	ctx->offset = 0;
	ctx->pos = 0;

	ctx->objs = NULL;
	ctx->top = -1;

	ctx->root = 0;
	ctx->info = 0;

	return ctx;
}


bool pdftoc_set_file(struct pdftoc *ctx, const char *file)
{
	assert(!ctx->file);
	ctx->file = fopen(file, "w");
	if (ctx->file)
		return 1;
	diag_perror(file);
	return 0;
}


static void add_object(struct pdftoc *ctx, int id, int gen, unsigned pos)
{
	struct object *obj;

	if (id > ctx->top) {
		ctx->objs = realloc_type_n(ctx->objs, struct object, id + 1);
		memset(ctx->objs + ctx->top + 1 , 0,
		    (id - ctx->top) * sizeof(struct object));
		ctx->top = id;
	}

	obj = ctx->objs + id;
	ctx->curr_obj = obj;
	obj->gen = gen;
	obj->pos = pos;
	obj->is_page = 0;
}


static bool parse_object(struct pdftoc *ctx, const char *s)
{
	int id, gen;
	int n = 0;

	if (sscanf(s, "%d %d obj%n", &id, &gen, &n) != 2 || !n)
		return 0;
	add_object(ctx, id, gen, ctx->pos);
	return 1;
}


static void line(struct pdftoc *ctx, const char *s)
{
	FILE *file = ctx->file ? ctx->file : stdout;

	switch (ctx->state) {
	case idle:
		if (parse_object(ctx, s)) {
			ctx->state = object;
			break;
		}
		if (strbegins(s, "xref")) {
			ctx->state = xref;
			break;
		}
		break;
	case object:
		if (strbegins(s, "endobj")) {
			ctx->state = idle;
			break;
		}
		if (strbegins(s, "<< /Type /Page") &&
		    !strbegins(s, "<< /Type /Pages")) {
			ctx->curr_obj->is_page = 1;
			break;
		}
		if (strbegins(s, "<< /Type /Catalog")) {
			ctx->state = catalog;
			break;
		}
		break;
	case catalog:
		if (strbegins(s, ">>")) {
			ctx->state = object;
			ctx->pos += fprintf(file,
			    "   /Outlines %u 0 R\n",
			    ctx->top + 1);
			break;
		}
		break;
	case xref:
		BUG("cannot be in xref");
	case trailer:
		if (sscanf(s, "   /Root %d 0 R", &ctx->root) == 1)
			break;
		if (sscanf(s, "   /Info %d 0 R", &ctx->info) == 1)
			break;
		break;
	default:
		BUG("invalid state %d", ctx->state);
	}
}


static void parse_buffer(struct pdftoc *ctx, bool do_write)
{
	FILE *file = ctx->file ? ctx->file : stdout;
	unsigned size, wrote;
	char *nl;

	while (ctx->state != xref) {
		nl = memchr(ctx->buf + ctx->offset, '\n', ctx->left);
		if (!nl)
			break;
		*nl = 0;
		size = nl - (ctx->buf + ctx->offset);
		line(ctx, ctx->buf + ctx->offset);
		*nl = '\n';
		if (ctx->state == xref)
			break;
		if (do_write) {
			wrote = fwrite(ctx->buf + ctx->offset, 1, size + 1,
			    file);
			if (wrote != size + 1)
				diag_pfatal("fwrite");
			ctx->pos += size + 1;
		}
		ctx->offset += size + 1;
		ctx->left -= size + 1;
	}
}


bool pdftoc_write(struct pdftoc *ctx, const void *data, unsigned length)
{
	char *buf;

	buf = alloc_size(ctx->left + length + 1);
	memcpy(buf, ctx->buf + ctx->offset, ctx->left);
	memcpy(buf + ctx->left, data, length);
	ctx->offset = 0;
	ctx->left += length;
	free(ctx->buf);
	ctx->buf = buf;

	parse_buffer(ctx, 1);

	return 1;
}


void pdftoc_title(struct pdftoc *ctx, const char *fmt, ...)
{
	va_list ap;
	struct title *t;

	t = alloc_type(struct title);
	va_start(ap, fmt);
	alloc_vprintf(&t->s, fmt, ap);
	va_end(ap);
	*ctx->next_title = t;
	t->next = NULL;
	ctx->next_title = &t->next;
	ctx->n_titles++;
}


static void write_trailer(struct pdftoc *ctx)
{
	FILE *file = ctx->file ? ctx->file : stdout;
	unsigned n = ctx->top + 1;
	const struct object *obj = ctx->objs;
	const struct title *t;
	unsigned outline, tail;
	int i;

	/* Outline root */

	outline = n;
	add_object(ctx, n, 0, ctx->pos);
	tail = fprintf(file,
	    "%u 0 obj\n<<\n"
	    "   /Count %u\n"
	    "   /First %u 0 R\n"
	    "   /Last %u 0 R\n"
	    ">>\nendobj\n",
	    n, ctx->n_titles, n + 1, n + ctx->n_titles);

	/* Outline items */

	n++;
	i = 0;
	for (t = ctx->titles; t; t = t->next) {
		assert(i <= ctx->top);
		while (!ctx->objs[i].is_page) {
			i++;
			assert(i <= ctx->top);
		}
		add_object(ctx, n, 0, ctx->pos + tail);
		tail += fprintf(file,
		    "%u 0 obj\n<<\n"
		    "   /Title (%s)\n"
		    "   /Parent %u 0 R\n",
		    n, t->s, outline);
		if (t != ctx->titles)
			tail += fprintf(file,
			    "   /Prev %u 0 R\n", n - 1);
		if (t->next)
			tail += fprintf(file,
			    "   /Next %u 0 R\n", n + 1);
		tail += fprintf(file,
		    "   /Dest [%d %u R /Fit]\n"
		    ">>\nendobj\n",
		    i, ctx->objs[i].gen);
		n++;
		i++;
	}

	/* xref table */

	fprintf(file, "xref\n0 %u\n", n);
	for (obj = ctx->objs; obj != ctx->objs + ctx->top + 1; obj++)
		fprintf(file,
		    "%010u %05u %c \n",
		    obj->pos, obj->pos ? 0 : 65535, obj->pos ? 'n' : 'f');

	fprintf(file,
	    "trailer\n"
	    "<< /Size %u\n"
	    "   /Root %u 0 R\n",
	    n, ctx->root);
	if (ctx->info)
		fprintf(file, "   /Info %u 0 R\n", ctx->info);
	fprintf(file, ">>\nstartxref\n%u\n%%%%EOF\n", ctx->pos + tail);
}


void pdftoc_end(struct pdftoc *ctx)
{
	FILE *file = ctx->file ? ctx->file : stdout;
	struct title *next;

	assert(ctx->state == xref);
	ctx->state = trailer;
	parse_buffer(ctx, 0);
	if (ctx->left) {
		fatal("%u bytes left in buffer at end\n", ctx->left);
		exit(1);
	}

	write_trailer(ctx);

	if (fclose(file) < 0)
		diag_pfatal("fclose");

	while (ctx->titles) {
		next = ctx->titles->next;
		free(ctx->titles->s);
		free(ctx->titles);
		ctx->titles = next;
	}
	free(ctx->buf);
	free(ctx->objs);
	free(ctx);
}
