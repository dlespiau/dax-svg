NULL = #

AM_CFLAGS =				\
	$(CASTET_CFLAGS)		\
	$(CASTET_DEBUG_CFLAGS)		\
	$(MAINTAINER_CFLAGS)		\
	-DG_LOG_DOMAIN=\"Castet\"	\
	$(NULL)

LDADD =						\
	$(CASTET_LT_LDFLAGS)			\
	-export-symbol-regex "^castet.*"	\
	$(NULL)

source_c =		\
	castet.c	\
	castet-debug.c	\
	$(NULL)

source_h =	\
	$(NULL)

source_h_private = 		\
	castet-debug.h		\
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
