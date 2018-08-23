UPLOAD = wpwrak@neo900.org:pub/eeshow/
SCP = rsync -e "neo900 --ssh" -v

FIGS = wt-start wt-plain wt-pin-type wt-glabel wt-index wt-history wt-diff \
       wt-diff-old wt-diff-new wt-diff-index wt-doc wt-cp

.PHONY:		upload

index.html:	$(FIGS:%=%.png)

%.png:		%.fig
		fig2dev -L png -S 4 $< >$@ || { rm -f $@; exit 1; }

wt-cp.png:	cp.png

#
# @@@ This doesn't work. ImageMagick produces one image per layer, apparently
# unaware of their stacking and the image operations going on between them.
#
# http://stackoverflow.com/questions/630405/converting-xcf-and-other-files-using-command-line-with-gimp
# also suggests xcf2png, which almost succeeds, but still produces an unusable
# image.
#
#cp.png:		cp.xcf
#		convert $< $@ || { rm -f $@; exit 1; }

upload:		$(FIGS:%=%.png)
		$(SCP) index.html $(FIGS:%=%.png) $(UPLOAD)
