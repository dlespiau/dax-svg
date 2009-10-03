include $(top_srcdir)/Makefile.decl

NULL =

noinst_PROGRAMS = $(TEST_PROGS)

INCLUDES = -I$(top_srcdir) $(CASTET_CFLAGS)
progs_ldadd =								\
	$(top_builddir)/src/libcastet-$(CASTET_API_VERSION).la	\
	$(CASTET_LIBS)

AM_CPPFLAGS = 			\
	-I$(top_srcdir)		\
	-I$(top_srcdir)/src	\
	$(NULL)

AM_CFLAGS = -g

TESTS_ENVIRONMENT = srcdir=$(srcdir)

TEST_PROGS          += test-parser
test_parser_SOURCES  = test-parser.c
test_parser_LDADD    = $(progs_ldadd)

TEST_PROGS          += test-types
test_types_SOURCES  = test-types.c
test_types_LDADD    = $(progs_ldadd)
