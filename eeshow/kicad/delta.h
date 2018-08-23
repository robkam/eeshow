/*
 * kicad/delta.h - Find differences in .sch files
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_DELTA_H
#define KICAD_DELTA_H

#include "kicad/sch.h"


bool sheet_eq(const struct sheet *a, const struct sheet *b, bool recurse);

void delta(const struct sheet *a, const struct sheet *b,
    struct sheet *res_a, struct sheet *res_b, struct sheet *res_ab);
void delta_free(struct sheet *d);

#endif /* !KICAD_DELTA_H */
