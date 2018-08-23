/*
 * kicad/pro.h - KiCad profile
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_PRO_H
#define	KICAD_PRO_H

#include "kicad/ext.h"
#include "file/file.h"


struct file_names *pro_parse_file(struct file *file,
    const struct file_names *fn_default);

#endif /* !KICAD_PRO_H */
