/*
 * gui/common.h - Common data structures and declarations
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef GUI_COMMON_H
#define	GUI_COMMON_H

#include <stdbool.h>

#include <cairo/cairo.h>
#include <gtk/gtk.h>

#include "gfx/cro.h"
#include "gfx/gfx.h"
#include "file/git-hist.h"
#include "kicad/lib.h"
#include "kicad/sch.h"
#include "gui/aoi.h"
#include "gui/over.h"


struct gui;
struct gui_hist;

struct gui_sheet {
	const struct sheet *sch;
	struct gui *gui;	/* back link */
	struct gui_hist *hist;	/* back link */
	struct gfx *gfx;

	int w, h;		/* in eeschema coordinates */
	int xmin, ymin;

	bool rendered;		/* 0 if we still have to render it */

	struct overlay *over;	/* current overlay */
	struct aoi *aois;	/* areas of interest; in schematics coord  */

	/* thumbnails, for index */
	struct gfx *gfx_thumb;		/* NULL if not yet rendered */
	cairo_surface_t *thumb_surf;	/* for caching */
	unsigned thumb_w, thumb_h;	/* size for which we cached */
	struct overlay *thumb_over;	/* thumb overlay */
	bool thumb_yellow;		/* change mark */

	struct gui_sheet *next;
};

struct gui_hist {
	struct gui *gui;	/* back link */
	struct vcs_hist *vcs_hist; /* NULL if not from repo */
	struct overlay *over;	/* current overlay */
	struct gui_sheet *sheets; /* NULL if failed */
	unsigned age;		/* 0-based; uncommitted or HEAD = 0 */

	struct pl_ctx *pl;	/* NULL if none or failed */

	/* caching support */
	void **oids;		/* file object IDs */
	unsigned libs_open;
	struct sch_ctx sch_ctx;
	struct lib lib;		/* combined library */
	bool identical;		/* identical with previous entry */

	bool skipped;		/* to synchronize thread display */

	struct gui_hist *next;
};

struct gui {
	GtkWidget *da;

	float scale;		/* pixels = eeschema * scale */
	int x, y;		/* center, in eeschema coordinates */

	struct gui_hist *hist;	/* revision history; NULL if none */
	struct vcs_history *vcs_history;
				/* underlying VCS data; NULL if none */

	enum gui_mode {
		showing_sheet,
		showing_history,
		showing_index,
	} mode;

	enum selecting {
		sel_only,	/* select the only revision we show */
		sel_split,	/* select revision to compare with */
		sel_new,	/* select the new revision */
		sel_old,	/* select the old revision */
	} selecting;

	struct overlay *sheet_overlays;
	struct overlay *hist_overlays;
	struct overlay *thumb_overlays;

	struct overlay *pop_overlays; /* pop-up dialogs */
	struct overlay *pop_underlays;
	int pop_x, pop_y;
	int pop_dx, pop_dy;
	const void *pop_origin;	/* item that created this pop-up */
	const char *glabel;	/* currenly highlighted glabel, or NULL */

	struct aoi *aois;	/* areas of interest; in canvas coord  */

	struct gui_sheet delta_a;
	struct gui_sheet delta_b;
	struct gui_sheet delta_ab;

	struct gui_sheet *curr_sheet;
				/* current sheet, always on new_hist */

	enum diff_mode {
		diff_new,	/* show only new sheet */
		diff_old,	/* show only old sheet */
		diff_delta,	/* show delta */
	} diff_mode;
	struct gui_hist *new_hist;
	struct gui_hist *old_hist; /* NULL if not comparing */

	const char *drag_text;	/* text being "dragged" (marked); or NULL */

	int hist_y_offset;	/* history list y offset */

	const struct vcs_hist *commit_hover;
				/* commit being hovered on; NULL if none */

	/* progress bar */
	int hist_size;		/* total number of revisions */
	unsigned progress;	/* progress counter */
	unsigned progress_scale;/* right-shift by this value */
};


#define	VCS_OVERLAYS_X		5
#define	VCS_OVERLAYS_Y		5

#define	POP_STACK_PADDING	5


/* progress.c */

void setup_progress_bar(struct gui *gui, GtkWidget *window);
void progress_update(struct gui *gui);

/* render.c */

extern bool use_delta;
extern enum gfx_extra show_extra;


void redraw(const struct gui *gui);
void render_sheet(struct gui_sheet *sheet);
void render_delta(struct gui *gui);
void render_setup(struct gui *gui);

/* glabel.c */

void add_glabel_aoi(struct gui_sheet *sheet, const struct sch_obj *obj);

/* comp.c */

void add_comp_aoi(struct gui_sheet *sheet, const struct sch_obj *obj);

/* sheet.c */

void go_to_sheet(struct gui *gui, struct gui_sheet *sheet);
void zoom_to_extents(struct gui *gui);
void do_revision_overlays(struct gui *gui);
void sheet_setup(struct gui *gui);

/* history.c */

void view_full_commit(struct gui *gui);
void commit_hover(struct gui *gui, const struct vcs_hist *vcs_hist);
void history_draw_event(const struct gui *gui, cairo_t *cr);
void show_history(struct gui *gui, enum selecting sel);

/* index.c */

void index_draw_event(const struct gui *gui, cairo_t *cr);
void index_resize(struct gui *gui);
void show_index(struct gui *gui);

/* gui.c */

struct gui_sheet *sheets(const struct gui *gui);
struct gui_sheet *find_corresponding_sheet(struct gui_sheet *pick_from,
    struct gui_sheet *ref_in, const struct gui_sheet *ref);
struct gui_sheet *current_sheet(const struct gui *gui);
void mark_aois(struct gui *gui, struct gui_sheet *sheet);

#endif /* !GUI_COMMON_H */
