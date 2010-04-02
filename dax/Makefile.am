include $(top_srcdir)/build/autotools/Makefile.am.silent

NULL = #
CLEANFILES = #

AM_CFLAGS =				\
	$(DAX_CFLAGS)		\
	$(DAX_DEBUG_CFLAGS)		\
	-DG_LOG_DOMAIN=\"Dax\"	\
	$(NULL)

LDADD =						\
	$(DAX_LT_LDFLAGS)			\
	-no-undefined				\
	-export-symbol-regex "^dax.*"	\
	$(NULL)

ENUMFILES = dax-enum-types.h dax-enum-types.c
STAMPFILES = stamp-dax-marshal.h stamp-dax-enum-types.h

BUILT_SOURCES = $(ENUMFILES)

source_c =				\
	dax-actor.c			\
	dax-animate-element.c	\
	dax-build-traverser.c	\
	dax-circle-element.c		\
	dax-core.c			\
	dax-debug.c			\
	dax-dom-character-data.c	\
	dax-dom-document.c		\
	dax-dom-element.c		\
	dax-dom-exception.c		\
	dax-dom-node.c		\
	dax-dom-text.c		\
	dax-document.c		\
	dax-desc-element.c		\
	dax-element.c		\
	dax-element-traversal.c	\
	dax-enum-types.c		\
	dax-g-element.c		\
	dax-gjs-context.c		\
	dax-handler-element.c	\
	dax-knot-sequence.c		\
	dax-paramspec.c		\
	dax-parser.c			\
	dax-path-element.c		\
	dax-polyline-element.c	\
	dax-rect-element.c		\
	dax-script-element.c		\
	dax-svg-element.c		\
	dax-svg-exception.c		\
	dax-title-element.c		\
	dax-traverser.c		\
	dax-types.c			\
	dax-utils.c			\
	dax-xml-event.c		\
	dax-xml-event-target.c	\
	dax-xml-event-listener.c	\
	clutter-shape.c			\
	$(NULL)

source_h =				\
	dax.h			\
	dax-actor.h			\
	dax-animate-element.h	\
	dax-build-traverser.h	\
	dax-circle-element.h		\
	dax-core.h			\
	dax-dom-forward.h		\
	dax-dom-character-data.h	\
	dax-dom-document.h		\
	dax-dom-element.h		\
	dax-dom-exception.h		\
	dax-dom-node.h		\
	dax-dom-text.h		\
	dax-document.h		\
	dax-desc-element.h		\
	dax-element.h		\
	dax-element-traversal.h	\
	dax-g-element.h		\
	dax-gjs-context.h		\
	dax-handler-element.h	\
	dax-knot-sequence.h		\
	dax-paramspec.h		\
	dax-parser.h			\
	dax-path-element.h		\
	dax-polyline-element.h	\
	dax-rect-element.h		\
	dax-script-element.h		\
	dax-svg-element.h		\
	dax-svg-exception.h		\
	dax-title-element.h		\
	dax-traverser.h		\
	dax-types.h			\
	dax-xml-event.h		\
	dax-xml-event-target.h	\
	dax-xml-event-listener.h	\
	clutter-shape.h			\
	$(NULL)

source_h_private = 		\
	dax-debug.h		\
	dax-dom-private.h	\
	dax-internals.h	\
	dax-private.h	\
	dax-utils.h		\
	dax-xml-private.h	\
	$(NULL)

lib_LTLIBRARIES = libdax-@DAX_API_VERSION@.la

daxdir = $(includedir)/dax-$(DAX_API_VERSION)/dax
dax_HEADERS =					\
	$(source_h)					\
	$(top_builddir)/dax/dax-enum-types.h		\
	$(NULL)

libdax_@DAX_API_VERSION@_la_SOURCES =	\
	$(source_c)				\
	$(source_h)				\
	$(source_h_private)			\
	$(NULL)

libdax_@DAX_API_VERSION@_la_CFLAGS =	\
	$(AM_CFLAGS)				\
	$(MAINTAINER_CFLAGS)			\
	$(NULL)

libdax_@DAX_API_VERSION@_la_LIBADD  = 	\
	$(DAX_LIBS)				\
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

# dax-enum-types.c should really depend on $(source_h) but it would make
# this rule trigger every single time a header is touched. We don't want that.
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

DISTCLEANFILES = 		\
	$(ENUMFILES) 		\
	$(NULL)

EXTRA_DIS =			\
	dax-enum-types.h.in	\
	dax-enum-types.c.in	\
	$(NULL)
