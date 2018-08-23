/*
 * gfx/fig.c - Generate FIG output for Eeschema items
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
#include <math.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "gfx/style.h"
#include "gfx/text.h"
#include "main.h"
#include "gfx/fig.h"


struct fig_ctx {
	FILE *file;
	bool skip;
};


/*
 * FIG works with 1/1200 in
 * KiCad works with mil
 * 1 point = 1/72 in
 */


static inline int cx(int x)
{
	return x * 1200 / 1000;
}


static inline int cy(int y)
{
	return y * 1200 / 1000;
}


static inline float pt(int x)
{
	return cx(x) * 72 * 1.5 / 1200.0;
}


/* ----- Schematics items -------------------------------------------------- */


static void fig_line(void *ctx, int sx, int sy, int ex, int ey,
    int color, unsigned layer)
{
	struct fig_ctx *fig = ctx;

	if (fig->skip)
		return;

	//   TypeStyle   FillCol AreaFil  Cap  FwdAr
	//     SubTy  Color   Pen   StyleV  Rad  BwdAr
	//         Thick  Depth        Join       Points
	fprintf(fig->file,
	    "2 1 2 %d %d 7 %d -1 -1 3.0 1 1 -1 0 0 2\n",
	    WIDTH_LINE, color, layer);
	fprintf(fig->file, "\t%d %d %d %d\n", cx(sx), cy(sy), cx(ex), cy(ey));
}


/* ----- General items ----------------------------------------------------- */


static void fig_rect(void *ctx, int sx, int sy, int ex, int ey,
    int color, int fill_color, unsigned layer)
{
	struct fig_ctx *fig = ctx;

	if (fig->skip)
		return;

	//   Type  Thick    Depth    StyleV  Rad
	//     SubTy  Color    Pen       Join   FwdAr
	//       Style   FillCol  AreaFil  Cap    BwdAr
	fprintf(fig->file,
	    "2 2 0 %d %d %d %d -1 %d 0.0 1 1 -1 0 0 5\n",
	    color == -1 ? 0 : WIDTH_COMP_DWG, color, fill_color, layer,
	    fill_color == -1 ? -1 : 20);
	fprintf(fig->file, "\t%d %d %d %d %d %d %d %d %d %d\n",
	    cx(sx), cy(sy), cx(ex), cy(sy), cx(ex), cy(ey), cx(sx), cy(ey),
	    cx(sx), cy(sy));
}


static void fig_poly(void *ctx,
    int points, const int x[points], const int y[points],
    int color, int fill_color, unsigned layer)
{
	struct fig_ctx *fig = ctx;
	int i;
	char ch = '\t';

	if (fig->skip)
		return;

	//   Type  Thick     Depth    StyleV  Rad
	//     SubTy  Color     Pen       Join   FwdAr
	//       Style   FillCol   AreaFil  Cap    BwdAr
	fprintf(fig->file,
	    "2 1 0 %d %d %d  %d -1 %d 0.0 1 1 -1 0 0 %d\n",
	    color == -1 ? 0 : WIDTH_COMP_DWG, color, fill_color, layer,
	    fill_color == -1 ? -1 : 20, points);
	for (i = 0; i != points; i++) {
		fprintf(fig->file, "%c%d %d", ch, cx(x[i]), cy(y[i]));
		ch = ' ';
	}
	fprintf(fig->file, "\n");
}


static void fig_circ(void *ctx, int x, int y, int r,
    int color, int fill_color, unsigned layer)
{
	struct fig_ctx *fig = ctx;

	if (fig->skip)
		return;

	//   Type  Thick    Depth   StyleV    Cx    Rx    Sx    Ex
	//     SubTy  Color    Pen       Dir      Cy    Ry    Sy    Ey
	//       Style   FillCol  AreaFil  Angle
	fprintf(fig->file,
	    "1 3 0 %d %d %d %d -1 %d 0.0 1 0.0 %d %d %d %d %d %d %d %d\n",
	    color == -1 ? 0 : WIDTH_COMP_DWG, color, fill_color, layer,
	    fill_color == -1 ? -1 : 20,
	    cx(x), cy(y), r, r,
	    cx(x), cy(y), cx(x) + r, cy(y));
}


static int ax(int x, int y, int r, int angle)
{
	float a = angle / 180.0 * M_PI;

	return cx(x + r * cos(a));
}


static int ay(int x, int y, int r, int angle)
{
	float a = angle / 180.0 * M_PI;

	return cy(y - r * sin(a));
}


static void fig_arc(void *ctx, int x, int y, int r, int sa, int ea,
    int color, int fill_color, unsigned layer)
{
	struct fig_ctx *fig = ctx;
	int ma = (sa + ea) / 2;

	if (fig->skip)
		return;

	//   Type  Thick    Depth   StyleV   FwdAr
	//     SubTy  Color    Pen       Cap   BwdAr
	//       Style   FillCol  AreaFil  Dir   points
	fprintf(fig->file,
	    "5 1 0 %d %d %d %d -1 %d 0.0 1 1 0 0 %d %d %d %d %d %d %d %d\n",
	    color == -1 ? 0 : WIDTH_COMP_DWG, color, fill_color, layer,
	    fill_color == -1 ? -1 : 20,
	    cx(x), cy(y),
	    ax(x, y, r, sa), ay(x, y, r, sa),
	    ax(x, y, r, ma), ay(x, y, r, ma),
	    ax(x, y, r, ea), ay(x, y, r, ea));
}


static void fig_tag(void *ctx, const char *s,
    int points, const int x[points], const int y[points])
{
	struct fig_ctx *fig = ctx;

	if (fig->skip)
		return;

	fprintf(fig->file, "# href=\"%s\" alt=\"\"\n", s);
	fig_poly(fig, points, x, y, COLOR_NONE, COLOR_NONE, 999);
}


static void fig_text(void *ctx, int x, int y, const char *s, unsigned size,
    enum text_align align, int rot, enum text_style style,
    unsigned color, unsigned layer)
{
	struct fig_ctx *fig = ctx;
	int font;

	if (fig->skip)
		return;

	switch (style) {
	case text_italic:
		font = FONT_HELVETICA_OBLIQUE;
		break;
	case text_bold:
		font = FONT_HELVETICA_BOLD;
		break;
	case text_bold_italic:
		font = FONT_HELVETICA_BOLDOB;
		break;
	default:
		font = FONT_HELVETICA;
		break;
	}

	//   Type   Depth     FontSiz Height
	//     Just    Pen       Angle    Length
	//       Color     Font     Flags     X  Y
	fprintf(fig->file,
	    "4 %u %d %d -1 %d %f %f 4 0.0 0.0 %d %d %s\\001\n",
	    align, color, layer, font,
	    pt(size), rot / 180.0 * M_PI, cx(x), cy(y), s);
}


static unsigned fig_text_width(void *ctx, const char *s, unsigned size,
    enum text_style style)
{
	unsigned n = 0;

	/*
	 * Note that we stretch the text size, so the ratio is larger than
	 * expressed here.
	 */

	while (*s) {
		n += (*s & 0xc0) != 0xc0 && *s != '~';
		s++;
	}
	return n * size * 1.0;

}


/* ----- FIG file header --------------------------------------------------- */


static void fig_header(FILE *file)
{
	fprintf(file, "#FIG 3.2\n");
	fprintf(file, "Landscape\n");
	fprintf(file, "Center\n");
	fprintf(file, "Metric\n");
	fprintf(file, "A4\n");
	fprintf(file, "100.00\n");
	fprintf(file, "Single\n");
	fprintf(file, "-2\n");
	fprintf(file, "1200 2\n");
}


static void fig_colors(FILE *file)
{
	unsigned i;

	for (i = 32; i != n_color_rgb; i++)
		fprintf(file, "0 %d #%06x\n", i, color_rgb[i]);

}


static bool apply_vars(char *buf, int n_vars, const char **vars)
{
	char *p;
	const char **var, *eq;
	int var_len, value_len;

	p = strchr(buf, '<');
	if (!p)
		return 0;
	for (var = vars; var != vars + n_vars; var++) {
		eq = strchr(*var, '=');
		assert(eq);
		var_len = eq - *var;
		if (strncmp(p + 1, *var, var_len))
			continue;
		value_len = strlen(eq + 1);
		memmove(p + value_len, p + var_len + 2,
		    strlen(p + var_len + 2) + 1);
		memcpy(p, eq + 1, value_len);
		return 1;
	}
	return 0;
}



static void *fig_init(void)
{
	struct fig_ctx *fig;

	fig = alloc_type(struct fig_ctx);
	fig->file = stdout;
	fig->skip = 0;
	return fig;
}


static bool fig_args(void *ctx, int argc, char *const *argv, const char *opts)
{
	struct fig_ctx *fig = ctx;
	static char *buf = NULL;
	static size_t n = 0;
	const char *output = NULL;
	const char *template = NULL;
	const char **vars = NULL;
	int n_vars = 0;
	const char *colon;
	char c;
	FILE *file;
	int lines_to_colors = 8;

	while ((c = getopt(argc, argv, opts)) != EOF)
		switch (c) {
		case 'o':
			colon = strchr(optarg, ':');
			output = colon ? colon + 1 : optarg;
			break;
		case 't':
			template = optarg;
			break;
		case 'D':
			if (!strchr(optarg, '='))
			    usage(*argv);
			n_vars++;
			vars = realloc_type_n(vars, const char *, n_vars);
			vars[n_vars - 1] = optarg;
			break;
		case '?':
			usage(*argv);
		default:
			break;
		}

	if (output && strcmp(output, "-")) {
		fig->file = fopen(output, "w");
		if (!fig->file)
			diag_pfatal(output);
	}

	if (!template) {
		fig_header(fig->file);
		fig_colors(fig->file);
		return 1;
	}

	file = fopen(template, "r");
	if (!file)
		diag_pfatal(template);
	while (getline(&buf, &n, file) > 0) {
		while (apply_vars(buf, n_vars, vars));
		fprintf(fig->file, "%s", buf);
		if (*buf == '#')
			continue;
		if (!--lines_to_colors)
			fig_colors(fig->file);
		/*
		 * @@@ known bug: if the template is empty, we won't output
		 * color 32.
		 */
	}
	fclose(file);

	return 1;
}


static void fig_set_extra(void *ctx, enum gfx_extra extra)
{
	struct fig_ctx *fig = ctx;

	fig->skip = extra;
}


static int fig_end(void *ctx, enum gfx_extra extra)
{
	struct fig_ctx *fig = ctx;

	/* @@@ support extra */
	if (fclose(fig->file) < 0)
		diag_pfatal("fclose");
	free(ctx);
	return 0;
}


/* ----- Operations -------------------------------------------------------- */


static const char *const fig_ext[] = { "fig" };

const struct gfx_ops fig_ops = {
	.ext		= fig_ext,
	.n_ext		= ARRAY_ELEMENTS(fig_ext),
	.opts		= "o:t:D:",

	.line		= fig_line,
	.rect		= fig_rect,
	.poly		= fig_poly,
	.circ		= fig_circ,
	.arc		= fig_arc,
	.text		= fig_text,
	.tag		= fig_tag,
	.text_width	= fig_text_width,
	.set_extra	= fig_set_extra,
	.init		= fig_init,
	.end		= fig_end,
	.args		= fig_args,
};
