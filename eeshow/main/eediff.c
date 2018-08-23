/*
 * main/eediff.c - Show differences between eeschema schematics
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
#include <unistd.h>
#include <string.h>

#include <cairo/cairo.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/diff.h"
#include "gfx/gfx.h"
#include "file/file.h"
#include "kicad/ext.h"
#include "kicad/pl.h"
#include "kicad/lib.h"
#include "kicad/sch.h"
#include "kicad/pro.h"
#include "version.h"
#include "main/common.h"
#include "main.h"


void usage(const char *name)
{
	fprintf(stderr,
"usage: %s [-o output.png] [-s scale] [-e] [-v ...]\n"
"       %*skicad_files kicad_files\n"
"       %s -V\n"
"       %s gdb ...\n"
"\n"
"  kicad_files  [rev:]file.ext\n"
"    ext        .pro, .lib, .sch, or .kicad_wks\n"
"    rev        git revision\n"
"  Libraries and page layout precede project and sheet. Sheet (if present)\n"
"  follows project. At least one of sheet or project must be present.\n"
"\n"
#if 0
"  -1    show only one sheet - do not recurse into sub-sheets\n"
#endif
"  -e    show extra information (e.g., pin types)\n"
"  -o output_file\n"
"        output file. Default is standard output.\n"
"  -s scale\n"
"        scale by indicated factor (default: 1.0)\n"
"  -v    increase verbosity of diagnostic output\n"
"  -V    print revision (version) number and exit\n"
"  gdb   run eeshow under gdb\n"
    , name, (int) strlen(name) + 1, "", name, name);
	exit(1);
}


#define	OPTIONS	"hvL:OPV"


int main(int argc, char **argv)
{
	char c;
	struct file_names file_names_a;
	struct file_names file_names_b;
	char *opts;
	struct gfx *gfx;
	int retval;

	run_under_gdb(argc, argv);

	alloc_printf(&opts, "%s%s", OPTIONS, diff_ops.opts);
	while ((c = getopt(argc, argv, opts)) != EOF)
		switch (c) {
		case 'v':
			verbose++;
			break;
		case 'L':
			suppress_page_layout = 1;
			break;
		case 'O':
			disable_overline = 1;
			break;
		case 'P':
			use_pango = 1;
			break;
		case 'V':
			fprintf(stderr, "%s %s\n", version, build_date);
			return 1;
		case 'h':
		case '?':
			usage(*argv);
		default:
			break;
		}

	if (argc - optind < 2)
		usage(*argv);

	classify_files_ab(&file_names_a, &file_names_b,
	    argv + optind, argc - optind);
	if (!file_names_a.pro && !file_names_a.sch)
		fatal("project or top sheet name required");
	if (!file_names_b.pro && !file_names_b.sch)
		fatal("project or top sheet name required");

	gfx = gfx_init(&diff_ops);
	if (!gfx_args(gfx, argc, argv, OPTIONS))
		return 1;

	/*
	 * @@@ new before old, to help diff_process_file keep track of things
	 */

	if (!diff_process_file(gfx_user(gfx), &file_names_b, argc, argv, opts))
		return 1;
	if (!diff_process_file(gfx_user(gfx), &file_names_a, argc, argv, opts))
		return 1;

	free_file_names(&file_names_a);
	free_file_names(&file_names_b);
	free(opts);

	retval = gfx_end(gfx, 0);

	file_cleanup();
	cairo_debug_reset_static_data();

	return retval;
}
