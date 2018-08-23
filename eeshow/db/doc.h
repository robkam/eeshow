/*
 * db/doc.h - Documentation database
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef DB_DOC_H
#define	DB_DOC_H

#include <stdbool.h>

#include "file/file.h"


bool doc_find(const char *ref,
    void (*fn)(void *user, const char *tag, const char *s), void *user);
bool doc_load(const char *name, const struct file *related);
void doc_free(void);

#endif /* !DB_DOC_H */
