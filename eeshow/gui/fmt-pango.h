/*
 * gui/fmt-pango.h - Format strings for Pango markup
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef GUI_FMT_PANGO_H
#define GUI_FMT_PANGO_H

#include <stdarg.h>


unsigned vsfmt_pango(char *buf, const char *fmt, va_list ap);
char *vfmt_pango(const char *fmt, va_list ap);
char *fmt_pango(const char *fmt, ...)
    __attribute__((format(printf, 1, 2)));

#endif /* !GUI_FMT_PANGO_H */
