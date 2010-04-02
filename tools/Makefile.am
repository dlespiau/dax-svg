NULL = #
TOOLS = #

bin_PROGRAMS = $(TOOLS)

progs_ldadd =							\
	$(top_builddir)/dax/libdax-$(DAX_API_VERSION).la	\
	$(DAX_LIBS)

AM_CPPFLAGS = 			\
	-I$(top_srcdir)		\
	-I$(top_srcdir)/dax	\
	$(DAX_CFLAGS)		\
	$(NULL)

TOOLS             += dax-viewer
dax_viewer_SOURCES = viewer-main.c
dax_viewer_LDADD   = $(progs_ldadd)
