/*
 * file/file.c - Open and read a file
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/git-file.h"
#include "file/file.h"


static char *getline_buf = NULL;
static size_t getline_n = 0;


void *file_oid(const struct file *file)
{
	if (!file->vcs)
		return NULL;
	return vcs_git_get_oid(file->vcs);
}


bool file_oid_eq(const void *a, const void *b)
{
	/*
	 * If both a and b are NULL, we don't have revision data and thus
	 * can't tell if they're identical.
	 */
	return a && b && vcs_git_oid_eq(a, b);
}


static void get_time(struct file *file)
{
	struct stat st;

	if (fstat(fileno(file->file), &st)) {
		diag_perror("fstat");
		file->mtime = 0;
	} else {
		file->mtime = st.st_mtime;
	}
}


bool file_cat(const struct file *file, void *user, const char *line)
{
	printf("%s\n", line);
	return 1;
}


char *file_graft_relative(const char *base, const char *name)
{
	const char *slash;
	char *res;
	unsigned len;

	if (*name == '/')
		return NULL;

	slash = strrchr(base, '/');
	if (!slash)
		return NULL;

	len = slash + 1 - base;
	res = alloc_size(len + strlen(name) + 1);
	memcpy(res, base, len);
	strcpy(res + len, name);

	return res;
}


static bool try_related(struct file *file)
{
	char *tmp;

	if (!file->related)
		return 0;

	tmp = file_graft_relative(file->related->name, file->name);
	if (!tmp)
		return 0;

	if (*file->name == '/')
		return 0;

	file->file = fopen(tmp, "r");
	if (!file->file) {
		free(tmp);
		return 0;
	}

	progress(1, "reading %s", tmp);

	free((char *) file->name);
	file->name = tmp;
	get_time(file);
	return 1;
}


/*
 * @@@ logic isn't quite complete yet. It should go something like this:
 *
 * - if there is no related item,
 *   - try file,
 *   - if there is a colon, try VCS,
 *   - give up
 * - if there is a related item,
 *   - if it is a VCS,
 *     - try the revision matching or predating (if different repo) that of the
 *       related repo,
 (     - fall through and try as if it was a file
 *   - try opening as file. If this fails,
 *     - if the name of the file to open is absolute, give up
 *     - try `dirname related`/file_ope_open
 *     - give up
 *
 * @@@ should we see if non-VCS file is in a VCS ? E.g.,
 * /home/my-libs/foo.lib 1234:/home/my-project/foo.sch
 * or maybe use : as indictor for VCS, i.e.,
 * :/home/my-libs/foo.lib ...
 *
 * @@@ explicit revision should always win over related.
 */

static void *open_vcs(struct file *file)
{
	char *colon;

	colon = strchr(file->name, ':');
	if (colon) {
		char *tmp;

		tmp = stralloc(file->name);
		tmp[colon - file->name] = 0;
		file->vcs = vcs_git_open(tmp, colon + 1,
		    file->related ? file->related->vcs : NULL);
		if (file->vcs) {
			free(tmp);
		} else {
			progress(2, "could not open %s:%s", tmp, colon + 1);
			return NULL;
		}
	} else {
		file->vcs = vcs_git_open(NULL, file->name,
		    file->related ? file->related->vcs : NULL);
		if (!file->vcs) {
			progress(2, "could not open %s", file->name);
			return NULL;
		}
	}
	file->mtime = vcs_git_time(file->vcs);
	return file->vcs;
}


static void file_init(struct file *file, const char *name,
    const struct file *related)
{
	file->name = stralloc(name);
	file->lineno = 0;
	file->related = related;
	file->file = NULL;
	file->vcs = NULL;
}


bool file_open_vcs(struct file *file, const char *name)
{
	file_init(file, name, NULL);
	file->vcs = open_vcs(file);
	return file->vcs;
}


bool file_open(struct file *file, const char *name, const struct file *related)
{
	file_init(file, name, related);

	if (related) {
		if (related->vcs)
			progress(2, "file_open: trying %s (related %s, vcs)",
			    name, related->name);
		else
			progress(2, "file_open: trying %s (related %s)",
			    name, related->name);
	} else {
		progress(2, "file_open: trying %s", name);
	}

	if (related && related->vcs) {
		file->vcs = open_vcs(file);
		if (file->vcs)
			return 1;
	}

	file->file = fopen(name, "r");
	if (file->file) {
		get_time(file);
		progress(1, "reading %s", name);
		return 1;
	}

	if (try_related(file))
		return 1;

	if (verbose)
		diag_perror(name);

	if (!strchr(name, ':')) {
		if (!verbose)
			diag_perror(name);
		goto fail;
	}

	file->vcs = open_vcs(file);
	if (file->vcs)
		return 1;

	error("could not open %s", name);
fail:
	free((char *) file->name);
	return 0;
}


bool file_search(struct file *file, const char *name,
    const char **search, unsigned n_search, const struct file *related)
{
	unsigned i;
	char *s;

	/*
	 * Caller should try file_open(file, name, ...) first, and only then
	 * try file_search. Thus, if there's a /, we already know that the file
	 * can't be found.
	 */
	if (*name == '/')
		return 0;

	for (i = 0; i != n_search; i++) {
		alloc_printf(&s, "%s/%s", search[i], name);
		if (file_open(file, s, related)) {
			free(s);
			return 1;
		}
		free(s);
	}
	return 0;
}


bool file_open_revision(struct file *file, const char *rev, const char *name,
    const struct file *related)
{
	if (!rev)
		return file_open(file, name, related);

	file_init(file, name, related);
	file->vcs = vcs_git_open(rev, name, related ? related->vcs : NULL);
	if (file->vcs) {
		file->mtime = vcs_git_time(file->vcs);
		return 1;
	}
	progress(2, "could not open %s at %s", name, rev);
	return 0;
}


bool file_read(struct file *file,
    bool (*parse)(const struct file *file, void *user, const char *line),
    void *user)
{
	char *nl;

	if (file->vcs)
		return vcs_read(file->vcs, file, parse, user);
	while (getline(&getline_buf, &getline_n, file->file) > 0) {
		nl = strchr(getline_buf, '\n');
		if (nl)
			*nl = 0;
		file->lineno++;
		if (!parse(file, user, getline_buf))
			return 0;
	}
	return 1;
}


void file_close(struct file *file)
{
	if (file->file)
		fclose(file->file);
	if (file->vcs)
		vcs_close(file->vcs);
	free((char *) file->name);
}


void file_cleanup(void)
{
	free(getline_buf);
}
