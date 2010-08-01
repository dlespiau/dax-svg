include $(top_srcdir)/build/autotools/Makefile.am.silent

include $(INTROSPECTION_MAKEFILE)

NULL = #
CLEANFILES = #

AM_CFLAGS =			\
	$(DOX_CFLAGS)		\
	$(DAX_DEBUG_CFLAGS)	\
	-DG_LOG_DOMAIN=\"Dox\"	\
	-DDOX_COMPILATION	\
	$(NULL)

LDADD =					\
	$(DOX_LT_LDFLAGS)		\
	-no-undefined			\
	-export-symbol-regex "^dox.*"	\
	$(NULL)

source_c =				\
	dox-navigator.c			\
	$(NULL)

source_h =				\
	dox-navigator.h			\
	$(NULL)

source_h_private = 			\
	dox-internals.h			\
	$(NULL)

lib_LTLIBRARIES = libdox-@DOX_API_VERSION@.la

doxdir = $(includedir)/dox-$(DOX_API_VERSION)/dox
dox_HEADERS =					\
	$(source_h)				\
	$(NULL)

libdox_@DOX_API_VERSION@_la_SOURCES =	\
	$(source_c)			\
	$(source_h)			\
	$(source_h_private)		\
	$(NULL)

libdox_@DOX_API_VERSION@_la_CFLAGS =	\
	$(AM_CFLAGS)			\
	$(MAINTAINER_CFLAGS)		\
	$(NULL)

libdox_@DOX_API_VERSION@_la_LIBADD  = 	\
	$(DOX_LIBS)			\
	$(NULL)

if HAVE_INTROSPECTION
GIRS =
TYPELIBS = $(GIRS:.gir=.typelib)
INTROSPECTION_GIRS = $(GIRS)

INTROSPECTION_COMPILER_ARGS = --includedir=$(builddir)/glib-controller

Dox-@DOX_API_VERSION@.gir: libdox-@DOX_API_VERSION@.la
Dox_@DOX_API_VERSION_@_gir_PACKAGES = gobject-2.0 clutter-1.0
Dox_@DOX_API_VERSION_@_gir_LIBS = libdox-@DOX_API_VERSION@.la
Dox_@DOX_API_VERSION_@_gir_FILES = $(dox_HEADERS) $(source_c)
Dox_@DOX_API_VERSION_@_gir_SCANNERFLAGS = --strip-prefix=Dox
Dox_@DOX_API_VERSION_@_gir_CFLAGS = $(AM_CFLAGS)
Dox_@DOX_API_VERSION_@_gir_INCLUDES = GObject-2.0

GIRS += Dox-@DAX_API_VERSION@.gir

endif

girdir = $(datadir)/gir-1.0
dist_gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(TYPELIBS)

CLEANFILES += $(dist_gir_DATA) $(typelibs_DATA)

CLEANFILES +=		\
	$(ENUMFILES)	\
	$(STAMPFILES)	\
	$(NULL)

DISTCLEANFILES = 	\
	$(ENUMFILES) 	\
	$(NULL)
