/*
 * misc/util.c - Common utility functions
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define	_GNU_SOURCE	/* for vasprintf */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "misc/util.h"


int alloc_vprintf(char **s, const char *fmt, va_list ap)
{
	int res;

	res = vasprintf(s, fmt, ap);
	if (res == -1) {
		perror("vasprintf");
		exit(1);
	}
	return res;
}



int alloc_printf(char **s, const char *fmt, ...)
{
	va_list ap;
	int res;

	va_start(ap, fmt);
	res = alloc_vprintf(s, fmt, ap);
	va_end(ap);
	return res;
}
