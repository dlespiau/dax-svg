
SUBDIRS = build data dox dax tools

if BUILD_TESTS
SUBDIRS += tests
endif

ACLOCAL_AMFLAGS = -I build/autotools

stats:
	@total=`find . -name "*.[ch]" | xargs wc -l | tail -n 1 | \
	        tr  -d [:space:][:alpha:]` && \
	 real=`sloccount dax/ dox/ tests/ tools/ | grep -e "^ansic:" | \
	        awk '{print $$2}'` && \
	 echo "Lines of code: $$total ($$real real)"

dax-$(DAX_API_VERSION).pc : dax.pc
	$(QUIET_GEN)cp $< $@

dox-$(DOX_API_VERSION).pc : dox.pc
	$(QUIET_GEN)cp $< $@

pcfiles =				\
	dax-$(DAX_API_VERSION).pc	\
	dox-$(DOX_API_VERSION).pc	\
	$(NULL)

pkgconfig_DATA = $(pcfiles)
pkgconfigdir = $(libdir)/pkgconfig

CLEANFILES = $(pcfiles)

.PHONY: stats
