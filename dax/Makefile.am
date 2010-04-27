include $(top_srcdir)/build/autotools/Makefile.am.silent

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
	dax-clutter-traverser.c		\
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
	dax-element.c			\
	dax-element-animate.c		\
	dax-element-circle.c		\
	dax-element-desc.c		\
	dax-element-g.c			\
	dax-element-handler.c		\
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
	dax-enum-types.c		\
	dax-gjs-context.c		\
	dax-knot-sequence.c		\
	dax-paramspec.c			\
	dax-parser.c			\
	dax-svg-exception.c		\
	dax-traverser.c			\
	dax-types.c			\
	dax-utils.c			\
	dax-xml-event.c			\
	dax-xml-event-target.c		\
	dax-xml-event-listener.c	\
	clutter-shape.c			\
	$(NULL)

source_h =				\
	dax.h				\
	dax-actor.h			\
	dax-clutter-traverser.h		\
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
	dax-document.h			\
	dax-element.h			\
	dax-element-animate.h		\
	dax-element-circle.h		\
	dax-element-desc.h		\
	dax-element-g.h			\
	dax-element-handler.h		\
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
	dax-gjs-context.h		\
	dax-knot-sequence.h		\
	dax-parser.h			\
	dax-svg-exception.h		\
	dax-traverser.h			\
	dax-types.h			\
	dax-xml-event.h			\
	dax-xml-event-target.h		\
	dax-xml-event-listener.h	\
	clutter-shape.h			\
	$(NULL)

source_h_private = 		\
	dax-debug.h		\
	dax-dom-private.h	\
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
BUILT_GIRSOURCES =

Dax-@DAX_API_VERSION@.gir: $(INTROSPECTION_SCANNER) libdax-@DAX_API_VERSION@.la
	$(QUIET_GEN)$(INTROSPECTION_SCANNER) -v \
		--namespace Dax --nsversion=@DAX_API_VERSION@ \
		$(AM_CFLAGS) \
		--c-include='dax/dax.h' \
		--include=GObject-2.0 \
		--include=Clutter-1.0 \
		--library=libdax-@DAX_API_VERSION@.la \
		--libtool="$(top_builddir)/libtool" \
	        --pkg gobject-2.0 \
	        --output $@ \
		$(dax_HEADERS) \
		$(source_c)

BUILT_GIRSOURCES += Dax-@DAX_API_VERSION@.gir
#
# INTROSPECTION_GIRDIR/INTROSPECTION_TYPELIBDIR aren't the right place to
# install anything - we need to install inside our prefix.
girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

typelibsdir = $(libdir)/girepository-1.0/

typelibs_DATA = $(BUILT_GIRSOURCES:.gir=.typelib)

%.typelib: %.gir $(INTROSPECTION_COMPILER)
	$(QUIET_GEN) \
	LD_LIBRARY_PATH=.libs$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH} \
	$(INTROSPECTION_COMPILER) \
		--includedir=$(srcdir) \
		--includedir=. \
		$(INTROSPECTION_COMPILER_OPTS) $< -o $(@F)

CLEANFILES += $(BUILT_GIRSOURCES) $(typelibs_DATA)

endif # HAVE_INTROSPECTION

CLEANFILES +=		\
	$(ENUMFILES)	\
	$(STAMPFILES)	\
	$(NULL)

DISTCLEANFILES = 	\
	$(ENUMFILES) 	\
	$(NULL)

EXTRA_DIS =			\
	dax-enum-types.h.in	\
	dax-enum-types.c.in	\
	$(NULL)
