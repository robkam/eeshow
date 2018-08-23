/*
 * main/eeplot.c - Plots eeschema schematics
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
#include <strings.h>	// for strncasecmp

#include <cairo/cairo.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/fig.h"
#include "gfx/cro.h"
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


static struct gfx_ops const *ops_list[] = {
	&fig_ops,
	&cro_png_ops,
	&cro_pdf_ops,
	&cro_ps_ops,
	&cro_eps_ops,
	&cro_svg_ops,
};


static const struct gfx_ops *find_by_output(int argc, char **argv)
{
	const char *arg, *dot, *colon, *ext;
	const struct gfx_ops *const *ops;
	int i;
	unsigned  n;

	for (i = 1; i != argc; i++)
		if (!strncmp(argv[i], "-o", 2))
			break;
	if (i == argc)
		return NULL;
	arg = argv[i] + 2;
	if (!*arg) {
		arg = argv[i + 1];
		if (!arg)
			fatal("-o requires an argument");
	}

	dot = strrchr(arg, '.');
	colon = strchr(arg, ':');
	if (colon && dot && dot < colon)
		usage(*argv);
	if (colon) {
		ext = arg;
		n = colon - arg;
	} else if (dot) {
		ext = dot + 1;
		n = strlen(ext);
	} else {
		fatal("either extension or \"type:\" prefix required for "
		    "output file");
	}

	for (ops = ops_list; ops != ARRAY_END(ops_list); ops++)
		for (i = 0; i != (*ops)->n_ext; i++)
			if (!strncasecmp((*ops)->ext[i], ext, n) &&
			    strlen((*ops)->ext[i]) == n)
				return *ops;
	fatal("graphics backend for \"%s\" not found\n", ext);
}


void usage(const char *name)
{
	fprintf(stderr,
"usage: %s -o [type:]output_file [-1] [-d date] [-e] [-v ...] [driver_opts]\n"
"       %*skicad_file ...\n"
"       %s -V\n"
"       %s gdb ...\n"
"\n"
"  kicad_file  [rev:]file.ext\n"
"    ext       .pro, .lib, .sch, or .kicad_wks\n"
"    rev       git revision\n"
"\n"
"  -1    show only one sheet - do not recurse into sub-sheets\n"
"  -d date\n"
"        use the specified string instead of date entries in sheets\n"
"  -e    show extra information (e.g., pin types)\n"
"  -o [type:]output_file\n"
"        output file. - for standard output. File type is derived from\n"
"        extension and can be overridden with type: prefix (fig, png, pdf,\n"
"        ps, eps).\n"
"  -v    increase verbosity of diagnostic output\n"
"  -V    print revision (version) number and exit\n"
"  gdb   run eeshow under gdb\n"
"\n"
"FIG files:\n"
"  [-t template.fig] [-D var=value ...]\n"
"\n"
"  -t template.fig  merge this file with generated output\n"
"  -D var=value     substitute \"<var>\" with \"value\" in template\n"
"\n"
"PNG, Postscript, and EPS files:\n"
"  [-s scale]\n"
"\n"
"  -s scale       scale by indicated factor (default: 1.0)\n"
"\n"
"PDF files:\n"
"  [-s scale] [-T]\n"
"\n"
"  see PNG for -s\n"
"  -n  show page numbers in the table of contents\n"
"  -T  do not add table of contents\n"
    , name, (int) strlen(name) + 1, "", name, name);
	exit(1);
}


#define	OPTIONS	"1d:ehvL:OPV"


int main(int argc, char **argv)
{
	struct lib lib;
	struct sch_ctx sch_ctx;
	struct file pro_file, sch_file;
	bool extra = 0;
	bool one_sheet = 0;
	struct pl_ctx *pl = NULL;
	char c;
	unsigned i;
	struct file_names file_names;
	struct file_names *fn = &file_names;
	char *opts;
	const struct gfx_ops *ops;
	struct gfx *gfx;
	int retval;

	run_under_gdb(argc, argv);

	ops = find_by_output(argc, argv);

	alloc_printf(&opts, "%s%s", OPTIONS, ops ? ops->opts : "");
	while ((c = getopt(argc, argv, opts)) != EOF)
		switch (c) {
		case '1':
			one_sheet = 1;
			break;
		case 'd':
			date_override = optarg;
			break;
		case 'e':
			extra = 1;
			break;
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
	free(opts);

	if (!ops)
		fatal("required option \"-o output\" is missing");

	if (argc - optind < 1)
		usage(*argv);

	classify_files(&file_names, argv + optind, argc - optind);
	if (!file_names.pro && !file_names.sch)
		fatal("project or top sheet name required");

	if (file_names.pro) {
		if (!file_open(&pro_file, file_names.pro, NULL))
			return 1;
		fn = pro_parse_file(&pro_file, &file_names);
	}

	gfx = gfx_init(ops);
	if (!gfx_args(gfx, argc, argv, OPTIONS))
		return 1;
	if (!gfx_multi_sheet(gfx))
		one_sheet = 1;

	sch_init(&sch_ctx, !one_sheet);
	if (!file_open(&sch_file, fn->sch, file_names.pro ? &pro_file : NULL))
		return 1;

	lib_init(&lib);
	for (i = 0; i != fn->n_libs; i++)
		if (!lib_parse_search(&lib, fn->libs[i], fn,
		    file_names.pro ? &pro_file : &sch_file))
			return 1;

	if (file_names.pro)
		file_close(&pro_file);

	pl = pl_parse_search(fn->pl, &sch_file);
	if (!pl)
		return 1;

	if (fn != &file_names) {
		free_file_names(fn);
		free(fn);
	}
	free_file_names(&file_names);

	if (!sch_parse(&sch_ctx, &sch_file, &lib, NULL))
		return 1;
	file_close(&sch_file);

	if (one_sheet) {
		sch_render(sch_ctx.sheets, gfx);
		if (pl)
			pl_render(pl, gfx, sch_ctx.sheets, sch_ctx.sheets);
	} else {
		const struct sheet *sheet;

		for (sheet = sch_ctx.sheets; sheet; sheet = sheet->next) {
			gfx_sheet_name(gfx, sheet->title);
			sch_render(sheet, gfx);
			if (pl)
				pl_render(pl, gfx, sch_ctx.sheets, sheet);
			if (sheet->next)
				gfx_new_sheet(gfx);
		}
	}
	retval = gfx_end(gfx, extra ? gfx_pin_type : 0);

	sch_free(&sch_ctx);
	lib_free(&lib);
	if (pl)
		pl_free(pl);

	file_cleanup();
	cairo_debug_reset_static_data();

	return retval;
}
