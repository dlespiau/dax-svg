include $(top_srcdir)/build/autotools/Makefile.decl

NULL =

noinst_PROGRAMS = $(TEST_PROGS)

progs_ldadd =							\
	$(top_builddir)/dax/libdax-$(DAX_API_VERSION).la	\
	$(DAX_LIBS)

AM_CPPFLAGS = 						\
	-I$(top_srcdir)					\
	-I$(top_srcdir)/dax				\
	-DDAX_ABS_TOP_SRCDIR=\"$(abs_top_srcdir)\"	\
	$(DAX_CFLAGS)					\
	$(MAINTAINER_CFLAGS)				\
	$(NULL)

AM_CFLAGS = -g

TESTS_ENVIRONMENT = srcdir=$(srcdir)

TEST_PROGS          += test-utils
test_utils_CPPFLAGS = $(AM_CPPFLAGS) -DDAX_COMPILATION
test_utils_SOURCES  =				\
	$(top_srcdir)/dax/dax-affine.c		\
	$(top_srcdir)/dax/dax-enum-types.c	\
	$(top_srcdir)/dax/dax-paramspec.c 	\
	$(top_srcdir)/dax/dax-types.c 		\
	$(top_srcdir)/dax/dax-utils.c 		\
	test-utils.c
test_utils_LDADD    = $(DAX_LIBS)

TEST_PROGS          += test-types
test_types_SOURCES  = test-types.c
test_types_LDADD    = $(progs_ldadd)

TEST_PROGS          += test-dom
test_dom_SOURCES     = test-dom.c test-common.h
test_dom_LDADD       = $(progs_ldadd)

TEST_PROGS          += test-parser
test_parser_SOURCES  = test-parser.c test-common.h
test_parser_LDADD    = $(progs_ldadd)

TEST_PROGS          += test-js
test_js_SOURCES      = test-js.c
test_js_LDADD        = $(progs_ldadd)
