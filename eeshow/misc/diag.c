/*
 * misc/diag.h - Diagnostics
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
#include <string.h>
#include <errno.h>

#include <git2.h>

#include "misc/util.h"
#include "misc/diag.h"


unsigned verbose = 0;

static int deferring = 0;
static char *deferred = NULL;


/* ----- Specialized diagnostic functions ---------------------------------- */


void diag_pfatal(const char *s)
{
	fatal("%s: %s", s, strerror(errno));
}


void diag_perror(const char *s)
{
	error("%s: %s", s, strerror(errno));
}


void pfatal_git(const char *s)
{
	const git_error *e = giterr_last();

	fatal("%s: %s", s, e->message);
}


void perror_git(const char *s)
{
	const git_error *e = giterr_last();

	error("%s: %s", s, e->message);
}


/* ----- Deferred errors --------------------------------------------------- */


void diag_defer_begin(void)
{
	deferring++;
}


static void diag_defer_finish(bool report)
{
	if (deferred && report)
		fprintf(stderr, "%s\n", deferred);
	free(deferred);
	deferred = NULL;
}


void diag_defer_end(bool report)
{
	if (!deferring--)
		BUG("deferring == 0");
	if (!deferring)
		diag_defer_finish(report);
}


/* ----- General diagnostic functions -------------------------------------- */


void fatal(const char *fmt, ...)
{
	va_list ap;

	diag_defer_finish(1);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(1); /* @@@ for now ... */
}


void error(const char *fmt, ...)
{
	va_list ap;

	if (deferred)
		free(deferred);
	va_start(ap, fmt);
	alloc_vprintf(&deferred, fmt, ap);
	va_end(ap);
	if (!deferring)
		diag_defer_finish(1);
}


void warning(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "warning: ");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}


void progress(unsigned level, const char *fmt, ...)
{
	va_list ap;

	if (level > verbose)
		return;
	va_start(ap, fmt);
	fprintf(stderr, "%*s", level * 2, "");
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
}
