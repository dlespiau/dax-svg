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

source_c =				\
	castet-actor.c			\
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
	castet-g-element.c		\
	castet-knot-sequence.c		\
	castet-parser.c			\
	castet-polyline-element.c	\
	castet-rect-element.c		\
	castet-svg-element.c		\
	castet-svg-exception.c		\
	castet-traverser.c		\
	$(NULL)

source_h =				\
	castet.h			\
	castet-actor.h			\
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
	castet-polyline-element.h	\
	castet-rect-element.h		\
	castet-svg-element.h		\
	castet-svg-exception.h		\
	castet-traverser.h		\
	$(NULL)

source_h_private = 		\
	castet-debug.h		\
	castet-internals.h	\
	$(NULL)

lib_LTLIBRARIES = libcastet-@CASTET_API_VERSION@.la

castetdir = $(includedir)/castet-$(CASTET_API_VERSION)/castet
castet_HEADERS =	\
	$(source_h)	\
	$(NULL)

libcastet_@CASTET_API_VERSION@_la_SOURCES =	\
	$(source_c)				\
	$(source_h)				\
	$(source_h_private)			\
	$(NULL)

libcastet_@CASTET_API_VERSION@_la_LIBADD  = 	\
	$(CASTET_LIBS)				\
	$(NULL)
