# castet package version number, (as distinct from shared library version)
# An odd micro number indicates in-progress development from Git
# An even micro number indicates a released version
#
# Making a point release:
# - increase castet_version_micro to the next even number
# - increase castet_interface_age to the next even number
#   UNLESS there was an API addition/deprecation, in which case
#   - set castet_interface_age to 0
# After the release:
# - increase castet_version_micro to the next odd number
# - increase castet_api_version to the next odd number
m4_define([castet_version_major], [0])
m4_define([castet_version_minor], [1])
m4_define([castet_version_micro], [0])

m4_define([castet_api_version], [0.1])

m4_define([castet_version],
          [castet_version_major.castet_version_minor.castet_version_micro])

# increase the interface age by 2 for each release; if the API changes,
# set to 0. interface_age and binary_age are used to create the soname
# of the shared object:
#
#  (<minor> * 100 + <micro>) - <interface_age>
#
# this allows using the same soname for different micro-releases in case
# no API was added or deprecated. for instance:
#
#   castet 1.2.0  -> 100 * 2 + 0  = 200, interface age = 0 -> 200
#   castet 1.2.2  -> 100 * 2 + 2  = 202, interface age = 2 -> 200
#   castet 1.2.4  -> 100 * 2 + 4  = 204, interface age = 4 -> 200
#   [ API addition, deprecation ]
#   castet 1.2.6  -> 100 * 2 + 6  = 206, interface age = 0 -> 206
#   castet 1.2.8  -> 100 * 2 + 8  = 208, interface age = 2 -> 206
#   castet 1.2.10 -> 100 * 2 + 10 = 210, interface age = 4 -> 206
#   ...
m4_define([castet_interface_age], [0])
m4_define([castet_binary_age], [m4_eval(100 * castet_version_minor + castet_version_micro)])

# Autoconf configuration
AC_PREREQ(2.59)

AC_INIT([castet],
        [castet_version],
        [http://bugzilla.moblin.org])
AC_CONFIG_MACRO_DIR([build/autotools])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src/castet.h])
AC_DISABLE_STATIC

# Automake configuration
AM_INIT_AUTOMAKE([-Wall foreign])
AM_CONFIG_HEADER(config.h)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# libtool configuration
LT_INIT

# export a few variables
CASTET_VERSION_MAJOR=castet_version_major
CASTET_VERSION_MINOR=castet_version_minor
CASTET_VERSION_MICRO=castet_version_micro
CASTET_API_VERSION=castet_api_version
AC_SUBST(CASTET_VERSION_MAJOR)
AC_SUBST(CASTET_VERSION_MINOR)
AC_SUBST(CASTET_VERSION_MICRO)
AC_SUBST(CASTET_API_VERSION)

m4_define([lt_current],  [m4_eval(castet_binary_age - castet_interface_age)])
m4_define([lt_revision], [castet_interface_age])
m4_define([lt_age],      [m4_eval(castet_binary_age - castet_interface_age)])
CASTET_LT_CURRENT=lt_current
CASTET_LT_REV=lt_revision
CASTET_LT_AGE=lt_age
CASTET_LT_VERSION="$CASTET_LT_CURRENT:$CASTET_LT_REV:$CASTET_LT_AGE"
CASTET_LT_LDFLAGS="-version-info $CASTET_LT_VERSION"

AC_SUBST(CASTET_LT_VERSION)
AC_SUBST(CASTET_LT_LDFLAGS)

# Check for programs
AC_PROG_CC
PKG_PROG_PKG_CONFIG
AC_PATH_PROG([GLIB_MKENUMS], [glib-mkenums])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Check for header files
AC_HEADER_STDC

PKG_CHECK_MODULES([CASTET], [glib-2.0 >= 2.16 gobject-2.0 clutter-1.0 clutter-gst-0.10])
AC_SUBST(CASTET_CFLAGS)
AC_SUBST(CASTET_LIBS)

dnl === Enable maintainer cflags ==============================================

m4_define([maintainer_flags_default], [yes])
AC_ARG_ENABLE([maintainer-flags],
              [AC_HELP_STRING([--enable-maintainer-flags=@<:@no/yes@:>@],
                              [Use strict compiler flags @<:@default=maintainer_flags_default@:>@])],
              [],
              [enable_maintainer_flags=maintainer_flags_default])

AS_IF([test "x$enable_maintainer_flags" = "xyes" && test "x$GCC" = "xyes"],
      [
        AS_COMPILER_FLAGS([MAINTAINER_CFLAGS],
                          ["-Wall -Wextra -Wno-unused-parameter
                           -Wno-ignored-qualifiers -Wshadow -Wcast-align
                           -Wno-missing-field-initializers -Wempty-body
                           -Wformat-security -Winit-self"])
      ]
)

AC_SUBST(MAINTAINER_CFLAGS)

dnl === Enable debug level ====================================================

m4_define([debug_default],
          [m4_if(m4_eval(castet_version_minor % 2), [1], [yes], [minimum])])

AC_ARG_ENABLE(
    [debug],
    [AC_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
    [Control Dax debugging level @<:@default=debug_default@:>@])],
    [],
    [enable_debug=debug_default])

AS_CASE([$enable_debug],

        [yes],
        [
          test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
          CASTET_DEBUG_CFLAGS="-DCASTET_ENABLE_DEBUG"
        ],

        [minimum],
        [CASTET_DEBUG_CFLAGS="-DCASTET_ENABLE_DEBUG -DG_DISABLE_CAST_CHECKS"],

        [no],
        [CASTET_DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"],

        [AC_MSG_ERROR([Unknown argument for --enable-debug])]
)

AC_SUBST(CASTET_DEBUG_CFLAGS)

AC_OUTPUT([
    Makefile
    build/Makefile
    build/autotools/Makefile
    src/Makefile
    tests/Makefile
    tools/Makefile
])

echo ""
echo "             Castet $VERSION"
echo "          ==================="
echo ""
echo "            prefix:   ${prefix}"
echo "    Compiler flags:   ${MAINTAINER_CFLAGS} ${CPPFLAGS}"
echo ""
echo "       debug level:   ${enable_debug}"
echo ""
