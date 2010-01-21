include $(top_srcdir)/build/autotools/Makefile.am.silent

NULL = #
CLEANFILES = #

AM_CFLAGS =				\
	$(CASTET_CFLAGS)		\
	$(CASTET_DEBUG_CFLAGS)		\
	-DG_LOG_DOMAIN=\"Castet\"	\
	$(NULL)

LDADD =						\
	$(CASTET_LT_LDFLAGS)			\
	-no-undefined				\
	-export-symbol-regex "^castet.*"	\
	$(NULL)

ENUMFILES = castet-enum-types.h castet-enum-types.c
STAMPFILES = stamp-castet-marshal.h stamp-castet-enum-types.h

BUILT_SOURCES = $(ENUMFILES)

source_c =				\
	castet-actor.c			\
	castet-animate-element.c	\
	castet-build-traverser.c	\
	castet-circle-element.c		\
	castet-core.c			\
	castet-debug.c			\
	castet-dom-character-data.c	\
	castet-dom-document.c		\
	castet-dom-element.c		\
	castet-dom-exception.c		\
	castet-dom-node.c		\
	castet-dom-text.c		\
	castet-document.c		\
	castet-desc-element.c		\
	castet-element.c		\
	castet-element-traversal.c	\
	castet-enum-types.c		\
	castet-g-element.c		\
	castet-gjs-context.c		\
	castet-handler-element.c	\
	castet-knot-sequence.c		\
	castet-parser.c			\
	castet-path-element.c		\
	castet-polyline-element.c	\
	castet-rect-element.c		\
	castet-script-element.c		\
	castet-svg-element.c		\
	castet-svg-exception.c		\
	castet-title-element.c		\
	castet-traverser.c		\
	castet-types.c			\
	castet-utils.c			\
	castet-xml-event.c		\
	castet-xml-event-target.c	\
	castet-xml-event-listener.c	\
	clutter-shape.c			\
	$(NULL)

source_h =				\
	castet.h			\
	castet-actor.h			\
	castet-animate-element.h	\
	castet-build-traverser.h	\
	castet-circle-element.h		\
	castet-core.h			\
	castet-dom-forward.h		\
	castet-dom-character-data.h	\
	castet-dom-document.h		\
	castet-dom-element.h		\
	castet-dom-exception.h		\
	castet-dom-node.h		\
	castet-dom-text.h		\
	castet-document.h		\
	castet-desc-element.h		\
	castet-element.h		\
	castet-element-traversal.h	\
	castet-g-element.h		\
	castet-gjs-context.h		\
	castet-handler-element.h	\
	castet-knot-sequence.h		\
	castet-parser.h			\
	castet-path-element.h		\
	castet-polyline-element.h	\
	castet-rect-element.h		\
	castet-script-element.h		\
	castet-svg-element.h		\
	castet-svg-exception.h		\
	castet-title-element.h		\
	castet-traverser.h		\
	castet-types.h			\
	castet-xml-event.h		\
	castet-xml-event-target.h	\
	castet-xml-event-listener.h	\
	clutter-shape.h			\
	$(NULL)

source_h_private = 		\
	castet-debug.h		\
	castet-internals.h	\
	castet-utils.h		\
	castet-xml-private.h	\
	castet-dom-private.h	\
	$(NULL)

lib_LTLIBRARIES = libcastet-@CASTET_API_VERSION@.la

castetdir = $(includedir)/castet-$(CASTET_API_VERSION)/castet
castet_HEADERS =					\
	$(source_h)					\
	$(top_builddir)/src/castet-enum-types.h		\
	$(NULL)

libcastet_@CASTET_API_VERSION@_la_SOURCES =	\
	$(source_c)				\
	$(source_h)				\
	$(source_h_private)			\
	$(NULL)

libcastet_@CASTET_API_VERSION@_la_CFLAGS =	\
	$(AM_CFLAGS)				\
	$(MAINTAINER_CFLAGS)			\
	$(NULL)

libcastet_@CASTET_API_VERSION@_la_LIBADD  = 	\
	$(CASTET_LIBS)				\
	$(NULL)

castet-enum-types.h: stamp-castet-enum-types.h
	@true
stamp-castet-enum-types.h: $(source_h) castet-enum-types.h.in Makefile
	$(QUIET_GEN)( $(GLIB_MKENUMS) \
		--template $(srcdir)/castet-enum-types.h.in \
	  $(source_h) ) >> xgen-ceth && \
	(cmp -s xgen-ceth castet-enum-types.h || cp xgen-ceth castet-enum-types.h) && \
	rm -f xgen-ceth && \
	echo timestamp > $(@F)

# castet-enum-types.c should really depend on $(source_h) but it would make
# this rule trigger every single time a header is touched. We don't want that.
castet-enum-types.c: castet-enum-types.h castet-enum-types.c.in
	$(QUIET_GEN)( $(GLIB_MKENUMS) \
		--template $(srcdir)/castet-enum-types.c.in \
	  $(source_h) ) >> xgen-cetc && \
	cp xgen-cetc castet-enum-types.c && \
	rm -f xgen-cetc

if HAVE_INTROSPECTION
BUILT_GIRSOURCES =

Castet-@CASTET_API_VERSION@.gir: $(INTROSPECTION_SCANNER) libcastet-@CASTET_API_VERSION@.la
	$(QUIET_GEN)$(INTROSPECTION_SCANNER) -v \
		--namespace Castet --nsversion=@CASTET_API_VERSION@ \
		$(AM_CFLAGS) \
		--c-include='castet/castet.h' \
		--include=GObject-2.0 \
		--include=Clutter-1.0 \
		--library=libcastet-@CASTET_API_VERSION@.la \
		--libtool="$(top_builddir)/libtool" \
	        --pkg gobject-2.0 \
	        --output $@ \
		$(castet_HEADERS) \
		$(source_c)

BUILT_GIRSOURCES += Castet-@CASTET_API_VERSION@.gir
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
	castet-enum-types.h.in	\
	castet-enum-types.c.in	\
	$(NULL)
