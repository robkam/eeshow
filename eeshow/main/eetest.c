/*
 * main/eetest.c - Subsystem tests (for development)
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

#include "misc/util.h"
#include "misc/diag.h"
#include "file/file.h"
#include "kicad/sexpr.h"
#include "gui/fmt-pango.h"
#include "file/git-hist.h"
#include "version.h"
#include "main/common.h"


static void sexpr(void)
{
	struct sexpr_ctx *parser = sexpr_new();
	struct expr *expr;
	char *buf = NULL;
	size_t n = 0;

	while (getline(&buf, &n, stdin) > 0)
		sexpr_parse(parser, buf);
	if (sexpr_finish(parser, &expr))
		dump_expr(expr);
	else
		exit(1);
}


static void usage(const char *name)
{
	fprintf(stderr,
"usage: %s [-v ...] -C [rev:]file\n"
"       %s [-v ...] -H path_into_repo\n"
"       %s -S\n"
"       %s -V\n"
"       %s gdb ...\n"
"\n"
"  -v    increase verbosity of diagnostic output\n"
"  -C    'cat' the file to standard output\n"
"  -H    show history of repository on standard output\n"
"  -S    parse S-expressions from stdin and dump to stdout\n"
"  -V    print revision (version) number and exit\n"
"  gdb   run eeshow under gdb\n"
    , name, name, name, name, name);
	exit(1);
}


int main(int argc, char **argv)
{
	const char *cat = NULL;
	const char *history = NULL;
	const char *fmt = NULL;
	int c;

	run_under_gdb(argc, argv);

	while ((c = getopt(argc, argv, "hvC:F:H:SV")) != EOF)
		switch (c) {
		case 'v':
			verbose++;
			break;
		case 'C':
			cat = optarg;
			break;
		case 'F':
			fmt = optarg;
			break;
		case 'H':
			history = optarg;
			break;
		case 'S':
			sexpr();
			return 0;
		case 'V':
			fprintf(stderr, "%s %s\n", version, build_date);
			return 1;
		case 'h':
		default:
			usage(*argv);
		}

	if (cat) {
		struct file file;

		if (argc != optind)
			usage(*argv);
		if (!file_open(&file, cat, NULL))
			return 1;
		if (!file_read(&file, file_cat, NULL))
			return 1;
		file_close(&file);
		return 0;
	}

	if (history) {
		dump_hist(vcs_git_history(history, 0));
		return 0;
	}

	if (fmt) {
		char *buf;

		buf = fmt_pango(fmt, argv[optind]);
		printf("\"%s\"\n", buf);
		return 0;
	}

	usage(*argv);

	return 0;	/* not reached */
}
