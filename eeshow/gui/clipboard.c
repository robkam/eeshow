/*
 * gui/clipboard.c - Copy text to clipboard
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>

#include "misc/diag.h"
#include "gui/clipboard.h"


#if 0
	/*
	 * This almost works, except that trying to paste on a different screen
	 * produces the following weird behaviour:
	 * - the paste attempt is ignored,
	 * - later, when selecting something else, that something else may get
	 *   pasted.
	 * This behaviour was observed when pasting into an xterm, but may be
	 * more universal.
	 */

	cb = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	gtk_clipboard_set_text(cb, s, -1);
	cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(cb, s, -1);

#endif


static void popen_s(const char *cmd, const char *s)
{
	FILE *p;

	p = popen(cmd, "w");
	if (!p) {
		diag_perror(cmd);
		return;
	}
	fprintf(p, "%s", s);
	if (pclose(p) < 0)
		diag_perror(cmd);
}


void copy_to_clipboard(const char *s)
{
	popen_s("xsel -i -p", s);
	popen_s("xsel -i -b", s);
}
