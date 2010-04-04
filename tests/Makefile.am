include $(top_srcdir)/build/autotools/Makefile.decl

NULL =

noinst_PROGRAMS = $(TEST_PROGS)

progs_ldadd =							\
	$(top_builddir)/dax/libdax-$(DAX_API_VERSION).la	\
	$(DAX_LIBS)

AM_CPPFLAGS = 			\
	-I$(top_srcdir)		\
	-I$(top_srcdir)/dax	\
	$(DAX_CFLAGS)		\
	$(NULL)

AM_CFLAGS = -g

TESTS_ENVIRONMENT = srcdir=$(srcdir)

TEST_PROGS          += test-utils
test_utils_SOURCES  = $(top_srcdir)/dax/dax-utils.c test-utils.c
test_utils_LDADD    = $(DAX_LIBS)

TEST_PROGS          += test-types
test_types_SOURCES  = test-types.c
test_types_LDADD    = $(progs_ldadd)

TEST_PROGS          += test-dom
test_dom_SOURCES     = test-dom.c
test_dom_LDADD       = $(progs_ldadd)

TEST_PROGS          += test-parser
test_parser_SOURCES  = test-parser.c
test_parser_LDADD    = $(progs_ldadd)
