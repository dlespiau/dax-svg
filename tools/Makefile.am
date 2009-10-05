NULL = #
TOOLS = #

bin_PROGRAMS = $(TOOLS)

progs_ldadd =							\
	$(top_builddir)/src/libcastet-$(CASTET_API_VERSION).la	\
	$(CASTET_LIBS)

AM_CPPFLAGS = 			\
	-I$(top_srcdir)		\
	-I$(top_srcdir)/src	\
	$(CASTET_CFLAGS)	\
	$(NULL)

TOOLS                 += castet-viewer
castet_viewer_SOURCES  = viewer-main.c
castet_viewer_LDADD    = $(progs_ldadd)
