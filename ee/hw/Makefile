NAME = neo900
#SCHTOC = ../../misc/schtoc/schtoc.pl

.PHONY:		pull sync sch sort brd pdf view
.PHONY:		test-conn
.PHONY:		upload upload-all upload-pdf upload-tar upload-zip clean

#----- Repository helpers -----------------------------------------------------

pull:
		git pull
		git submodule update --init
		$(MAKE) -C kicad-libs/components
		-$(MAKE) -C modules
		-$(MAKE) -C kicad-libs/modules

sync:
		git submodule update
		$(MAKE) -C kicad-libs/components
		-$(MAKE) -C kicad-libs/modules

#----- Editing and PDF generation ---------------------------------------------

sch:
		[ -e kicad-libs ] || ln -s ../../../qi/kicad-libs .
		eeschema $(NAME).sch
		perl ./sortsheets.pl -q -xy $(NAME).sch || \
		    { echo "sheet order changed -- run \"make sort\"" 1>&2; \
		    exit 1; }

sort:
		perl ./sortsheets.pl -xy $(NAME).sch >_tmp && \
		    mv $(NAME).sch $(NAME).bak && mv _tmp $(NAME).sch

brd:
		pcbnew $(NAME).kicad_pcb
		sed -i '/(diff_pair_/d' $(NAME).kicad_pcb

#
# Warning: "make pdf' generates the PDF from the current files, which means
# that also the date comes from the files, not the repository. If you want
# to change this, use HEAD:neo900.pro instead of neo900.pdf
#

pdf:
		TZ=UTC eeplot -n -d "%{%F %T}s" neo900.pro -o neo900.pdf
#		$(SCHTOC) -n -z neo900.sch neo900.pdf >neo900-toc.pdf || \
#		    { rm -f neo900-toc.pdf; exit 1; }

#----- Documentation ----------------------------------------------------------

REVCOUNT ?= 30
SHEETS = $(shell sed 's/^F1 "\([^"]*\)".*/\1/p;d' neo900.sch)

view:		docdb.out
		EESHOW_VIEWER=eeshow-viewer \
		  EESHOW_PDF_VIEWER=$${EESHOW_PDF_VIEWER:-xpdf} \
		  eeshow -N $(REVCOUNT) -d docdb.out neo900.pro

docdb.out:	docdb.in $(SHEETS)
		./gendocdb.pl -c -i $^ >$@ || { rm -f $@; exit 1; }

#----- Snapshots --------------------------------------------------------------

DATE = $(shell gdate -u +"%Y%m%d-%H%MZ")
COMMIT = $(shell git log -1 --format='%h' -s)
PDF_NAME = neo900-$(DATE)-$(COMMIT).pdf
PDF_DIR = pub/werner/tmp/ee/pdf
TAR_NAME = neo900-$(DATE)-$(COMMIT).tar.bz2
TAR_DIR = pub/werner/tmp/ee/tar
TAR_EXTRA = kicad-libs/components/gencon.lib
ZIP_NAME = neo900-$(DATE)-$(COMMIT).zip
ZIP_DIR = pub/werner/tmp/ee/zip
ZIP_EXTRA = $(TAR_EXTRA)

upload:		upload-pdf

upload-all:	upload-pdf upload-tar upload-zip

upload-pdf:
		TZ=UTC eeplot -n -d "%{%F %T}s" HEAD:neo900.pro -o $(PDF_NAME)
		neo900 $(PDF_NAME) $(PDF_DIR)
		echo "cd $(PDF_DIR) && ln -sf $(PDF_NAME) neo900.pdf" | neo900
		rm -f $(PDF_NAME)

upload-tar:
		tar cfj $(TAR_NAME) \
		    --no-recursion --xform 's|^|neo900-$(DATE)-$(COMMIT)/|' \
		    `git ls-files` \
		    `git submodule --quiet foreach \
		    'git ls-files | sed "s|^|$$path/|"'` $(TAR_EXTRA)
		neo900 $(TAR_NAME) $(TAR_DIR)
		echo "cd $(TAR_DIR) && ln -sf $(TAR_NAME) neo900.tar.bz2" | \
		    neo900
		rm -f $(TAR_NAME)

upload-zip:
		zip $(ZIP_NAME) \
		    -nw -X \
		    `git ls-files` \
		    `git submodule --quiet foreach \
		    'git ls-files | sed "s|^|$$path/|"'` $(ZIP_EXTRA)
		neo900 $(ZIP_NAME) $(ZIP_DIR)
		echo "cd $(ZIP_DIR) && ln -sf $(ZIP_NAME) neo900.zip" | neo900
		rm -f $(ZIP_NAME)

#----- Automated tests --------------------------------------------------------

test-conn:
		../scripts/conncmp.pl neo900.net P1502 neo900.net P1501 \
		    GND=GND3 HB_AUDIO_GND=GND3 %=%_U \
		    VBUS=NC OTG_DP=NC OTG_DM=NC OTG_ID=NC \
		    USB_HB_DP=NC USB_HB_DM=NC
		../scripts/conncmp.pl neo900.net P1504 neo900.net P1503 \
		    GND=GND3 HB_AUDIO_GND=GND3 %=%_U \
		    VINTANA2=2V7_U \
		    VIB_P=NC VIB_M=NC
		../scripts/conncmp.pl neo900.net CON1601 neo900.net CON1701 \
		    HB_AUDIO_GND=GND2 HB_AUDIO_GND=NFC_GND VBUS=VBUS_OTG \
		    VHB=HB_GPIO_D_VDD
		../scripts/conncmp.pl neo900.net CON1602 neo900.net CON1702 \
		    HB_AUDIO_GND=GND2

#----- Cleanup ----------------------------------------------------------------

clean:
		rm -f _tmp
		rm -f dbdoc.out
