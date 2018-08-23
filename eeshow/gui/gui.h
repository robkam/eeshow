/*
 * gui/gui.h - GUI for eeshow
 *
 * Written 2016-2017 by Werner Almesberger
 * Copyright 2016-2017 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_GUI_H
#define	GUI_GUI_H

#include <stdbool.h>

#include "kicad/ext.h"


/*
 * @@@ ought to pass this to run_gui, then share via common.h, but let's be
 * pragmatic :-)
 */

extern unsigned comp_pop_width;

int run_gui(const struct file_names *fn, bool recurse, int limit,
    const char **commands, unsigned n_commands);

#endif /* !GUI_GUI_H */
