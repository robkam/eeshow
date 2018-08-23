/*
 * main/common.c - Functions common to more than one form of invocation
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
#include <string.h>

#include "misc/util.h"
#include "main/common.h"


void run_under_gdb(int argc, char *const *argv)
{
	char **args;

	if (argc < 2 || strcmp(argv[1], "gdb"))
		return;

	args = alloc_type_n(char *, argc + 2);
	args[0] = "gdb";
	args[1] = "--args";
	args[2] = *argv;
	memcpy(args + 3, argv + 2, sizeof(char *) * (argc - 1));
	execvp("gdb", args);
	perror(*argv);
	exit(1);
}
