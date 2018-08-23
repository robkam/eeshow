#
# Makefile - build eeshow
#
# Written 2016 by Werner Almesberger
# Copyright 2016 by Werner Almesberger
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

SHELL = /bin/bash

PREFIX ?= /usr/local

NAME = eeshow

OBJS_KICAD = \
	kicad/sch-parse.o kicad/sch-render.o kicad/lib-parse.o \
	kicad/lib-render.o kicad/dwg.o kicad/delta.o kicad/sexpr.o \
	kicad/pl-parse.o kicad/pl-render.o kicad/ext.o kicad/pro.o
OBJS_FILE = \
	file/file.o file/git-util.o file/git-file.o file/git-hist.o
OBJS_MISC = \
	misc/diag.o misc/util.o

EESHOW_OBJS = main/eeshow.o main/common.o \
	$(OBJS_KICAD) \
	gui/gui.o gui/over.o gui/style.o gui/aoi.o gui/fmt-pango.o gui/input.o \
	gui/progress.o gui/glabel.o gui/sheet.o gui/history.o gui/render.o \
	gui/help.o gui/icons.o gui/index.o gui/timer.o gui/pop.o gui/comp.o \
	gui/viewer.o gui/clipboard.o gui/view.o \
	$(OBJS_FILE) \
	gfx/style.o gfx/fig.o gfx/record.o gfx/cro.o gfx/diff.o gfx/gfx.o \
	gfx/text.o gfx/misc.o gfx/pdftoc.o \
	db/doc.o \
	$(OBJS_MISC)
EEPLOT_OBJS = main/eeplot.o main/common.o \
	$(OBJS_KICAD) \
	$(OBJS_FILE) \
	gfx/style.o gfx/fig.o gfx/record.o gfx/cro.o gfx/gfx.o \
	gfx/text.o gfx/misc.o gfx/pdftoc.o \
	$(OBJS_MISC)
EEDIFF_OBJS = main/eediff.o main/common.o \
	$(OBJS_KICAD) \
	$(OBJS_FILE) \
	gfx/style.o gfx/record.o gfx/cro.o gfx/diff.o gfx/gfx.o \
	gfx/text.o gfx/misc.o gfx/pdftoc.o \
	$(OBJS_MISC)
EETEST_OBJS = main/eetest.o main/common.o \
	kicad/sexpr.o \
	kicad/pl-render.o gfx/gfx.o gfx/text.o gfx/misc.o \
	gui/fmt-pango.o \
	$(OBJS_FILE) \
	$(OBJS_MISC)

# we need this for "clean" and dependencies
OBJS = $(EESHOW_OBJS) $(EEPLOT_OBJS) $(EEDIFF_OBJS) $(EETEST_OBJS)

ICONS = delta diff

CFLAGS = -g  -Wall -Wextra -Wno-unused-parameter -Wshadow \
	 -Wmissing-prototypes -Wmissing-declarations \
	 -I. \
	 `pkg-config --cflags cairo` \
	 `pkg-config --cflags libgit2` \
	 `pkg-config --cflags gtk+-3.0`
LDLIBS = -lm \
	 `pkg-config --libs cairo` \
	 `pkg-config --libs libgit2` \
	 `pkg-config --libs gtk+-3.0`

GIT_VERSION = $(shell git log -1 --format='%h' -s .)
GIT_STATUS = $(shell [ -z "`git status -s -uno`" ] || echo +)
BUILD_DATE = $(shell date -u +'%Y%m%d-%H:%MZ')

CFLAGS += -DVERSION='"$(GIT_VERSION)$(GIT_STATUS)"' \
	  -DBUILD_DATE='"$(BUILD_DATE)"'

include Makefile.c-common

.PHONY:		test neo900 sch test testref png pngref pdf diff view newref
.PHONY:		leak install uninstall

all::		eeshow eeplot eediff eetest

#
# We use order-only prerequisites to serialize linking. This is to ensure that
# a parallel make run won't try to compile version.o more than once at the same
# time, possibly producing corrupted output.
#

eeshow:		$(EESHOW_OBJS)
		$(MAKE) -B version.o
		$(CC) -o $@ $(EESHOW_OBJS) version.o $(LDLIBS)

eeplot:		$(EEPLOT_OBJS) | eeshow
		$(MAKE) -B version.o
		$(CC) -o $@ $(EEPLOT_OBJS) version.o $(LDLIBS)

eediff:		$(EEDIFF_OBJS) | eeplot
		$(MAKE) -B version.o
		$(CC) -o $@ $(EEDIFF_OBJS) version.o $(LDLIBS)

eetest:		$(EETEST_OBJS) | eediff
		$(MAKE) -B version.o
		$(CC) -o $@ $(EETEST_OBJS) version.o $(LDLIBS)

#----- Help texts -------------------------------------------------------------

help.inc:	help.txt Makefile
		$(BUILD) sed 's/"/\\"/g;s/.*/"&\\n"/' $< >$@ || \
		    { rm -f $@; exit 1; }

gui/help.c:	help.inc

clean::
		rm -f help.inc

#----- Icons ------------------------------------------------------------------

icons/%.hex:	icons/%.fig Makefile
		$(BUILD) fig2dev -L png -S 4 -Z 0.60 $< | \
		    convert - -transparent white - | \
		    hexdump -v -e '/1 "0x%x, "' >$@; \
		    [ "$${PIPESTATUS[*]}" = "0 0 0" ] || { rm -f $@; exit 1; }

gui/icons.c:	$(ICONS:%=icons/%.hex)

clean::
		rm -f $(ICONS:%=icons/%.hex)

#----- Default page layout ----------------------------------------------------

pl-default.inc:	page_layout_default_description.cpp
		$(BUILD) sed '1,/^extern/d;s/\\\\n/ /' $< >$@ || \
		    { rm -f $@; exit 1; }

kicad/pl-parse.c: pl-default.inc

clean::
		rm -f pl-default.inc

#----- Test sheet -------------------------------------------------------------

sch:
		eeschema test.sch

test:		eeplot
		./eeplot -o out.fig test.lib test.sch
		fig2dev -L png -m 2 out.fig _out.png
		[ ! -r ref.png ] || \
		    compare -metric AE ref.png _out.png _diff.png || \
		    qiv -t -R -D _diff.png ref.png _out.png

testref:	eeplot
		./eeplot -o fig:- test.lib test.sch | \
		    fig2dev -L png -m 2 >ref.png

png:		eeplot
		./eeplot -o _out.png -s 2 test.lib test.sch
		[ ! -r pngref.png ] || \
		    compare -metric AE pngref.png _out.png _diff.png || \
		    qiv -t -R -D _diff.png pngref.png _out.png

pngref:		eeplot
		./eeplot -o pngref.png -s 2 test.lib test.sch

clean::
		rm -f out.fig _out.png _diff.png

#----- Render Neo900 schematics -----------------------------------------------

NEO900_HW = ../../n9/ee/hw
KICAD_LIBS = ../../qi/kicad-libs/components

SHEET ?= 12

neo900:		eeplot
		./eeplot -o out.fig $(NEO900_HW)/neo900.lib \
		    $(KICAD_LIBS)/powered.lib \
		    $(NEO900_HW)/neo900_SS_$(SHEET).sch

neo900.pdf:	eeplot sch2pdf neo900-template.fig
		./sch2pdf -o $@ -t neo900-template.fig \
		    $(NEO900_HW)/neo900.lib $(KICAD_LIBS)/powered.lib \
		    $(NEO900_HW)/neo900.sch

pdf:		eeplot
		./eeplot -o neo900.pdf $(NEO900_HW)/neo900.pro

#----- Regression test based on Neo900 schematics -----------------------------

diff:		eeplot
		test/genpng test out
		test/comp test || $(MAKE) view

view:
		qiv -t -R -D `echo test/_diff*.png | \
		    sed 's/\([^ ]*\)_diff\([^ ]*\)/\1_diff\2 \1ref\2 \1out\2/g'`

newref:
		test/genpng test ref

#----- Memory leak checking ---------------------------------------------------

SUPP = dl-init cairo-font

leak:		eediff
		valgrind --leak-check=full --show-leak-kinds=all \
		    --num-callers=50 \
		    $(SUPP:%=--suppressions=%.supp) \
		    ./eediff $(NEO900_HW)/neo900.pro \
		      $(NEO900_HW)/neo900.pro >/dev/null
#		    ./eeplot -o pdf:- -N 1 $(NEO900_HW)/neo900.pro >/dev/null
#		    ./eeplot -o png:- -N 1 $(NEO900_HW)/neo900.pro >/dev/null

#----- Install / uninstall ---------------------------------------------------

install:	eeshow eeplot eediff eeshow-viewer
		mkdir -p $(DESTDIR)/$(PREFIX)/bin/
		install -m 755 $^ $(DESTDIR)/$(PREFIX)/bin/

uninstall:
		for n in eeshow eeplot eediff; do \
		    rm -f $(DESTDIR)/$(PREFIX)/bin/$$n; done

#----- Cleanup ----------------------------------------------------------------

spotless::
		rm -f eeshow eeplot eediff eetest
