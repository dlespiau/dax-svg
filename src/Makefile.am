include $(top_srcdir)/build/autotools/Makefile.am.silent

NULL = #

AM_CFLAGS =				\
	$(CASTET_CFLAGS)		\
	$(CASTET_DEBUG_CFLAGS)		\
	$(MAINTAINER_CFLAGS)		\
	-DG_LOG_DOMAIN=\"Castet\"	\
	$(NULL)

LDADD =						\
	$(CASTET_LT_LDFLAGS)			\
	-no-undefined				\
	-export-symbol-regex "^castet.*"	\
	$(NULL)

ENUMFILES = castet-enum-types.c castet-enum-types.h
STAMPFILES = stamp-castet-marshal.h stamp-castet-enum-types.h

BUILT_SOURCES = $(ENUMFILES)

source_c =				\
	castet-actor.c			\
	castet-animate-element.c	\
	castet-build-traverser.c	\
	castet-core.c			\
	castet-debug.c			\
	castet-dom-document.c		\
	castet-dom-element.c		\
	castet-dom-exception.c		\
	castet-dom-node.c		\
	castet-document.c		\
	castet-desc-element.c		\
	castet-element.c		\
	castet-element-traversal.c	\
	castet-enum-types.c		\
	castet-g-element.c		\
	castet-knot-sequence.c		\
	castet-parser.c			\
	castet-path-element.c		\
	castet-polyline-element.c	\
	castet-rect-element.c		\
	castet-svg-element.c		\
	castet-svg-exception.c		\
	castet-traverser.c		\
	castet-types.c			\
	clutter-shape.c			\
	$(NULL)

source_h =				\
	castet.h			\
	castet-actor.h			\
	castet-animate-element.h	\
	castet-build-traverser.h	\
	castet-core.h			\
	castet-dom-document.h		\
	castet-dom-element.h		\
	castet-dom-exception.h		\
	castet-dom-node.h		\
	castet-document.h		\
	castet-desc-element.h		\
	castet-element.h		\
	castet-element-traversal.h	\
	castet-g-element.h		\
	castet-knot-sequence.h		\
	castet-parser.h			\
	castet-path-element.h		\
	castet-polyline-element.h	\
	castet-rect-element.h		\
	castet-svg-element.h		\
	castet-svg-exception.h		\
	castet-traverser.h		\
	clutter-shape.h			\
	castet-types.h			\
	$(NULL)

source_h_private = 		\
	castet-debug.h		\
	castet-internals.h	\
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

CLEANFILES =		\
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
