/*
 * kicad/pro.c - KiCad profile
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
#include <string.h>
#include <assert.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/file.h"
#include "kicad/ext.h"
#include "kicad/pro.h"


struct pro_ctx {
	enum pro_state {
		pro_idle,
		pro_libs,	// [eeschema/libraries]
		pro_editor,	// [schematic_editor]
		pro_eeschema,	// [eeschema]
	} state;
	struct file_names *fn;
};


static void add_libdir(struct file_names *fn, char *s)
{
	char *p;

	while (1) {
		p = strtok(s, ":;");
		if (!p)
			break;
		s = NULL;
		fn->n_search++;
		fn->search =
		    realloc_type_n(fn->search, const char *, fn->n_search);
		fn->search[fn->n_search - 1] = stralloc(p);
	}
}


static bool pro_parse_line(const struct file *file,
    void *user, const char *line)
{
	struct pro_ctx *pro = user;
	char *s;

	if (strbegins(line, "[eeschema/libraries]")) {
		pro->state = pro_libs;
		return 1;
	}
	if (strbegins(line, "[schematic_editor]")) {
		pro->state = pro_editor;
		return 1;
	}
	if (strbegins(line, "[eeschema]")) {
		pro->state = pro_eeschema;
		return 1;
	}
	if (*line == '[') {
		pro->state = pro_idle;
		return 1;
	}

	switch (pro->state) {
	case pro_idle:
		break;
	case pro_libs:
		if (sscanf(line, "LibName%*u=%ms", &s) == 1) {
			char *dot;

			pro->fn->n_libs++;
			pro->fn->libs = realloc_type_n(pro->fn->libs,
			    const char *, pro->fn->n_libs);
			dot = strrchr(s, '.');
			if (!dot || strcmp(dot, ".lib")) {
				s = realloc_size(s, strlen(s) + 5);
				strcat(s, ".lib");
			}
			pro->fn->libs[pro->fn->n_libs - 1] = s;
			return 1;
		}
		break;
	case pro_editor:
		if (sscanf(line, "PageLayoutDescrFile=%ms", &s) == 1) {
			free((void *) pro->fn->pl);
			pro->fn->pl = s;
			return 1;
		}
		break;
	case pro_eeschema:
		if (sscanf(line, "LibDir=%ms", &s) == 1) {
			add_libdir(pro->fn, s);
			free(s);
			return 1;
		}
		break;
	default:
		BUG("invalid state %d", pro->state);
	}
	return 1;
}


struct file_names *pro_parse_file(struct file *file,
    const struct file_names *fn_default)
{
	struct pro_ctx pro = {
		.state	= pro_idle,
		.fn	= clone_file_names(fn_default),
	};

	if (!file_read(file, pro_parse_line, &pro)) {
		free_file_names(pro.fn);
		free(pro.fn);
		return NULL;
	}

	if (!pro.fn->sch) {
		char *s, *dot;

		assert(pro.fn->pro);
		s = stralloc(pro.fn->pro);
		dot = strrchr(s, '.');
		assert(!strcmp(dot, ".pro"));
		strcpy(dot, ".sch");
		pro.fn->sch = s;
	}
	return pro.fn;
}
