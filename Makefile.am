
SUBDIRS = build data dax tools

if BUILD_TESTS
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I build/autotools

stats:
	@total=`find . -name "*.[ch]" | xargs wc -l | tail -n 1 | \
	        tr  -d [:space:][:alpha:]` && \
	 real=`sloccount dax/ tests/ tools/ | grep -e "^ansic:" | \
	        awk '{print $$2}'` && \
	 echo "Lines of code: $$total ($$real real)"

dax-$(DAX_API_VERSION).pc : dax.pc
	$(QUIET_GEN)cp $< $@

pkgconfig_DATA = dax-$(DAX_API_VERSION).pc
pkgconfigdir = $(libdir)/pkgconfig

CLEANFILES = dax-$(DAX_API_VERSION).pc

.PHONY: stats
