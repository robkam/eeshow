/*
 * kicad/ext.h - Identify files by their extension
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_EXT_H
#define	KICAD_EXT_H

enum ext {
	ext_unknown,
	ext_project,
	ext_sch,
	ext_lib,
	ext_pl,		/* .kicad_wks, work sheet */
};


struct file_names {
	const char *pro;	/* just one allowed, may be NULL */
	const char *sch;	/* just one allowed, may be NULL */
	const char *pl;		/* just one allowed, may be NULL */
	const char **libs;
	unsigned n_libs;
	const char **search;	/* search paths from LibDir */
	unsigned n_search;
};


enum ext identify(const char *path);
void classify_files(struct file_names *fn, char *const *args,
    unsigned n_args);
void classify_files_ab(struct file_names *a, struct file_names *b,
    char *const *args, unsigned n_args);
struct file_names *clone_file_names(const struct file_names *fn);
void free_file_names(struct file_names *fn);

#endif /* !KICAD_EXT_H */
