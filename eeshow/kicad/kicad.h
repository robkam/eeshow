/*
 * kicad/kicad.h - Common KiCad-related definitons
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef KICAD_KICAD_H
#define	KICAD_KICAD_H

/* According to common/gestfich.cpp:KicadDatasPath */

#define	KICAD_DATAS_PATH(append)	\
    "/usr/share/kicad"		append,	\
    "/usr/local/share/kicad"	append,	\
    "/usr/local/kicad/share"	append,	\
    "/usr/local/kicad"		append

#define	DEFAULT_LIBRARY_PATHS	KICAD_DATAS_PATH("/library")
#define DEFAULT_TEMPLATE_PATHS	KICAD_DATAS_PATH("/template")

#endif /* !KICAD_KICAD_H */
