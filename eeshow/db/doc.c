/*
 * db/doc.c - Documentation database
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
#include <ctype.h>
#include <string.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/file.h"
#include "db/doc.h"


static struct doc {
	const char *ref;	/* component reference */
	const char *tag;	/* document type (shown in pop-up) */
	const char *s;		/* string to pass to viewer */
	struct doc *next;
} *docs = NULL, *curr_doc = NULL;


/* ----- Lookup ------------------------------------------------------------ */


bool doc_find(const char *ref,
    void (*fn)(void *user, const char *tag, const char *s), void *user)
{
	const struct doc *d;
	bool found = 0;
	for (d = docs; d; d = d->next)
		if (!strcmp(d->ref, ref)) {
			fn(user, d->tag, d->s);
			found = 1;
		}
	return found;
}


/* ----- Parsing ----------------------------------------------------------- */


static const char *untail(const char *s)
{
	const char *start = s;
	const char *last = s;
	char *new;

	while (*s) {
		if (!isspace(*s))
			last = s;
		s++;
	}
	new = alloc_size(last - start + 2);
	memcpy(new, start, last - start + 1);
	new[last - start + 1] = 0;
	return new;
}


static bool parse_line(const struct file *file,
    void *user, const char *line)
{
	bool *start = user;
	const char *s = line;
	struct doc *new;

	while (1) {
		switch (*s) {
		case 0:			/* empty line(s) separate(s) records */
			*start = 1;
			return 1;
		case '\r':		/* ignore leading whitespace */
		case ' ':
		case '\t':
			s++;
			continue;
		case '#':		/* ignore comment-only line */
			return 1;
		default:
			break;
			
		}
		break;
	}

	if (*start) {
		new = alloc_type(struct doc);
		new->ref = new->tag = new->s = NULL;
		new->next = NULL;
		if (curr_doc)
			curr_doc->next = new;
		else
			docs = new;
		curr_doc = new;
		*start = 0;
	}
	if (curr_doc->s)
		warning("ignoring extra line for \"%s\"", curr_doc->ref);
	else if (curr_doc->tag)
		curr_doc->s = untail(line);
	else if (curr_doc->ref)
		curr_doc->tag = untail(line);
	else
		curr_doc->ref = untail(line);
	return 1;
}


bool doc_load(const char *name, const struct file *related)
{
	struct file file;
	bool start = 1;

	if (!file_open(&file, name, related))
		return 0;
	file_read(&file, parse_line, &start);
	file_close(&file);
	return 1;
}


/* ----- Cleanup ----------------------------------------------------------- */


void doc_free(void)
{
	struct doc *next;

	while (docs) {
		next = docs->next;
		free((void *) docs->ref);
		free((void *) docs->tag);
		free((void *) docs->s);
		free(docs);
		docs = next;
	}
	curr_doc = NULL;
}
