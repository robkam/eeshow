/*
 * gui/viewer.c - Document viewer
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "misc/diag.h"
#include "gui/viewer.h"


#define	VIEWER_VAR	"EESHOW_VIEWER"
#define	DEFAULT_VIEWER	"xdg-open"


void viewer(const char *url)
{
	const char *v;
	pid_t pid;

	v = getenv(VIEWER_VAR);
	if (!v)
		v = DEFAULT_VIEWER;
	pid = fork();
	if (pid < 0)
		diag_perror("fork");
	if (pid)
		return;
	execlp(v, v, url, NULL);
	perror(v);
	_exit(1);
}
