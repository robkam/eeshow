MODULES = bga luxeon3020 smd-dil vemd10940f CPL-WBF-00D3 MNR-series YFC uDFN-9 M50-315XX DF9 101-00271-82

.SUFFIXES:	.fpd .mod
.PHONY:		all clean spotless

all:		$(MODULES:%=%.mod)

.fpd.mod:
		fped -k $< $@

clean:
		rm -f *~*.fpd

spotless:	clean
		rm -f $(MODULES:%=%.mod)
