include $(top_srcdir)/build/autotools/Makefile.am.silent

include $(INTROSPECTION_MAKEFILE)

NULL = #
CLEANFILES = #

AM_CFLAGS =			\
	$(DAX_CFLAGS)		\
	$(DAX_DEBUG_CFLAGS)	\
	-DG_LOG_DOMAIN=\"Dax\"	\
	-DDAX_COMPILATION	\
	$(NULL)

LDADD =					\
	$(DAX_LT_LDFLAGS)		\
	-no-undefined			\
	-export-symbol-regex "^dax.*"	\
	$(NULL)

ENUMFILES = dax-enum-types.h dax-enum-types.c
STAMPFILES = stamp-dax-marshal.h stamp-dax-enum-types.h

BUILT_SOURCES = $(ENUMFILES)

source_c =				\
	dax-actor.c			\
	dax-affine.c			\
	dax-cache.c			\
	dax-cache-entry.c		\
	dax-core.c			\
	dax-debug.c			\
	dax-dom-character-data.c	\
	dax-dom-core.c			\
	dax-dom-document.c		\
	dax-dom-element.c		\
	dax-dom-exception.c		\
	dax-dom-node.c			\
	dax-dom-text.c			\
	dax-document.c			\
	dax-dom-utils.c			\
	dax-element.c			\
	dax-element-animate.c		\
	dax-element-animate-transform.c	\
	dax-element-animation.c		\
	dax-element-circle.c		\
	dax-element-desc.c		\
	dax-element-g.c			\
	dax-element-handler.c		\
	dax-element-image.c		\
	dax-element-line.c		\
	dax-element-path.c		\
	dax-element-polyline.c		\
	dax-element-rect.c		\
	dax-element-script.c		\
	dax-element-svg.c		\
	dax-element-text.c		\
	dax-element-title.c		\
	dax-element-traversal.c		\
	dax-element-tspan.c		\
	dax-element-video.c		\
	dax-enum-types.c		\
	dax-gjs-context.c		\
	dax-gjs-function-listener.c	\
	dax-gjs-udom.c			\
	dax-group.c			\
	dax-knot-sequence.c		\
	dax-paramspec.c			\
	dax-parser.c			\
	dax-shape.c			\
	dax-svg-exception.c		\
	dax-traverser.c			\
	dax-traverser-clutter.c		\
	dax-types.c			\
	dax-udom-svg-timer.c		\
	dax-utils.c			\
	dax-xml-event.c			\
	dax-xml-event-target.c		\
	dax-xml-event-listener.c	\
	clutter-shape.c			\
	$(NULL)

source_h =				\
	dax.h				\
	dax-actor.h			\
	dax-cache.h			\
	dax-cache-entry.h		\
	dax-core.h			\
	dax-dom.h			\
	dax-dom-character-data.h	\
	dax-dom-core.h			\
	dax-dom-document.h		\
	dax-dom-element.h		\
	dax-dom-forward.h		\
	dax-dom-exception.h		\
	dax-dom-node.h			\
	dax-dom-text.h			\
	dax-dom-utils.h			\
	dax-document.h			\
	dax-element.h			\
	dax-element-animate.h		\
	dax-element-animate-transform.h	\
	dax-element-animation.h		\
	dax-element-circle.h		\
	dax-element-desc.h		\
	dax-element-g.h			\
	dax-element-handler.h		\
	dax-element-image.h		\
	dax-element-line.h		\
	dax-element-path.h		\
	dax-element-polyline.h		\
	dax-element-rect.h		\
	dax-element-script.h		\
	dax-element-svg.h		\
	dax-element-text.h		\
	dax-element-title.h		\
	dax-element-traversal.h		\
	dax-element-tspan.h		\
	dax-element-video.h		\
	dax-gjs-context.h		\
	dax-gjs-function-listener.h	\
	dax-group.h			\
	dax-knot-sequence.h		\
	dax-parser.h			\
	dax-shape.h			\
	dax-svg-exception.h		\
	dax-traverser.h			\
	dax-traverser-clutter.h		\
	dax-types.h			\
	dax-udom-svg-timer.h		\
	dax-xml-forward.h		\
	dax-xml-event.h			\
	dax-xml-event-target.h		\
	dax-xml-event-listener.h	\
	clutter-shape.h			\
	$(NULL)

source_h_private = 		\
	dax-affine.h		\
	dax-cache.h		\
	dax-cache-entry.h	\
	dax-debug.h		\
	dax-dom-private.h	\
	dax-gjs-udom.h		\
	dax-internals.h		\
	dax-paramspec.h		\
	dax-private.h		\
	dax-utils.h		\
	dax-xml-private.h	\
	$(NULL)

lib_LTLIBRARIES = libdax-@DAX_API_VERSION@.la

daxdir = $(includedir)/dax-$(DAX_API_VERSION)/dax
dax_HEADERS =					\
	$(source_h)				\
	$(top_builddir)/dax/dax-enum-types.h	\
	$(NULL)

libdax_@DAX_API_VERSION@_la_SOURCES =	\
	$(source_c)			\
	$(source_h)			\
	$(source_h_private)		\
	$(NULL)

libdax_@DAX_API_VERSION@_la_CFLAGS =	\
	$(AM_CFLAGS)			\
	$(MAINTAINER_CFLAGS)		\
	$(NULL)

libdax_@DAX_API_VERSION@_la_LIBADD  = 	\
	$(DAX_LIBS)			\
	$(NULL)

dax-enum-types.h: stamp-dax-enum-types.h
	@true
stamp-dax-enum-types.h: $(source_h) dax-enum-types.h.in Makefile
	$(QUIET_GEN)( $(GLIB_MKENUMS) \
		--template $(srcdir)/dax-enum-types.h.in \
	  $(source_h) ) >> xgen-ceth && \
	(cmp -s xgen-ceth dax-enum-types.h || cp xgen-ceth dax-enum-types.h) && \
	rm -f xgen-ceth && \
	echo timestamp > $(@F)

dax-enum-types.c: dax-enum-types.h dax-enum-types.c.in
	$(QUIET_GEN)( $(GLIB_MKENUMS) \
		--template $(srcdir)/dax-enum-types.c.in \
	  $(source_h) ) >> xgen-cetc && \
	cp xgen-cetc dax-enum-types.c && \
	rm -f xgen-cetc

if HAVE_INTROSPECTION
GIRS =
TYPELIBS = $(GIRS:.gir=.typelib)
INTROSPECTION_GIRS = $(GIRS)

INTROSPECTION_COMPILER_ARGS = --includedir=$(builddir)/glib-controller

Dax-@DAX_API_VERSION@.gir: libdax-@DAX_API_VERSION@.la
Dax_0_2_gir_PACKAGES = gobject-2.0 clutter-1.0
Dax_0_2_gir_LIBS = libdax-@DAX_API_VERSION@.la
Dax_0_2_gir_FILES = $(dax_HEADERS) $(source_c)
Dax_0_2_gir_SCANNERFLAGS = \
	--strip-prefix=Dax \
	--add-include-path=$(builddir)/glib-controller
Dax_0_2_gir_CFLAGS = $(AM_CFLAGS)
Dax_0_2_gir_INCLUDES = GObject-2.0 Clutter-1.0

GIRS += Dax-@DAX_API_VERSION@.gir

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

EXTRA_DIST =			\
	dax-enum-types.h.in	\
	dax-enum-types.c.in	\
	$(NULL)
