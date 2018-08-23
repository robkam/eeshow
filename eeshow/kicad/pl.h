/*
 * kicad/pl.h - KiCad page layout
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_PL_H
#define	KICAD_PL_H

#include <stdbool.h>

#include "gfx/gfx.h"
#include "file/file.h"
#include "kicad/sch.h"


struct pl_ctx;


extern bool suppress_page_layout;
extern const char *date_override;


void pl_render(struct pl_ctx *pl, struct gfx *gfx,
    const struct sheet *sheets, const struct sheet *sheet);

struct pl_ctx *pl_parse(struct file *file);
struct pl_ctx *pl_parse_search(const char *name, const struct file *related);
void pl_free(struct pl_ctx *pl);

#endif /* !KICAD_PL_H */
