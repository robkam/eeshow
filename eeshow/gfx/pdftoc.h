/*
 * gfx/pdftoc.h - PDF writer with TOC generation
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef GFX_PDFTOC_H
#define	GFX_PDFTOC_H

#include <stdbool.h>


struct pdftoc;

struct pdftoc *pdftoc_begin(void);
bool pdftoc_set_file(struct pdftoc *ctx, const char *file);
bool pdftoc_write(struct pdftoc *ctx, const void *data, unsigned length);
void pdftoc_title(struct pdftoc *ctx, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
void pdftoc_end(struct pdftoc *ctx);

#endif /* !GFX_PDFTOC_H */
