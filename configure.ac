# dax package version number, (as distinct from shared library version)
# An odd micro number indicates in-progress development from Git
# An even micro number indicates a released version
#
# Making a point release:
# - increase dax_version_micro to the next even number
# - increase dax_interface_age to the next even number
#   UNLESS there was an API addition/deprecation, in which case
#   - set dax_interface_age to 0
# After the release:
# - increase dax_version_micro to the next odd number
# - increase dax_api_version to the next odd number
m4_define([dax_version_major], [0])
m4_define([dax_version_minor], [1])
m4_define([dax_version_micro], [0])

m4_define([dax_api_version], [0.1])

m4_define([dax_version],
          [dax_version_major.dax_version_minor.dax_version_micro])

# increase the interface age by 2 for each release; if the API changes,
# set to 0. interface_age and binary_age are used to create the soname
# of the shared object:
#
#  (<minor> * 100 + <micro>) - <interface_age>
#
# this allows using the same soname for different micro-releases in case
# no API was added or deprecated. for instance:
#
#   dax 1.2.0  -> 100 * 2 + 0  = 200, interface age = 0 -> 200
#   dax 1.2.2  -> 100 * 2 + 2  = 202, interface age = 2 -> 200
#   dax 1.2.4  -> 100 * 2 + 4  = 204, interface age = 4 -> 200
#   [ API addition, deprecation ]
#   dax 1.2.6  -> 100 * 2 + 6  = 206, interface age = 0 -> 206
#   dax 1.2.8  -> 100 * 2 + 8  = 208, interface age = 2 -> 206
#   dax 1.2.10 -> 100 * 2 + 10 = 210, interface age = 4 -> 206
#   ...
m4_define([dax_interface_age], [0])
m4_define([dax_binary_age], [m4_eval(100 * dax_version_minor + dax_version_micro)])

# Autoconf configuration
AC_PREREQ(2.59)

AC_INIT([dax], [dax_version], [http://bugzilla.moblin.org])
AC_CONFIG_MACRO_DIR([build/autotools])
AC_CONFIG_AUX_DIR([build/autotools])
AC_CONFIG_SRCDIR([dax/dax.h])

# Automake configuration
AM_INIT_AUTOMAKE([1.10 -Wall -Wno-portability foreign no-define check-news])
AM_CONFIG_HEADER([config.h])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AS_AM_REALLY_SILENT

# libtool configuration
LT_PREREQ([2.2.6])
LT_INIT([win32-dll disable-static])

# export a few variables
DAX_VERSION_MAJOR=dax_version_major
DAX_VERSION_MINOR=dax_version_minor
DAX_VERSION_MICRO=dax_version_micro
DAX_API_VERSION=dax_api_version
AC_SUBST(DAX_VERSION_MAJOR)
AC_SUBST(DAX_VERSION_MINOR)
AC_SUBST(DAX_VERSION_MICRO)
AC_SUBST(DAX_API_VERSION)

m4_define([lt_current],  [m4_eval(dax_binary_age - dax_interface_age)])
m4_define([lt_revision], [dax_interface_age])
m4_define([lt_age],      [m4_eval(dax_binary_age - dax_interface_age)])
DAX_LT_CURRENT=lt_current
DAX_LT_REV=lt_revision
DAX_LT_AGE=lt_age
DAX_LT_VERSION="$DAX_LT_CURRENT:$DAX_LT_REV:$DAX_LT_AGE"
DAX_LT_LDFLAGS="-version-info $DAX_LT_VERSION"

AC_SUBST(DAX_LT_VERSION)
AC_SUBST(DAX_LT_LDFLAGS)

# Check for programs
AC_PROG_CC
PKG_PROG_PKG_CONFIG
AC_PATH_PROG([GLIB_MKENUMS], [glib-mkenums])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Check for header files
AC_HEADER_STDC

PKG_CHECK_MODULES([DAX], [glib-2.0 >= 2.16 gobject-2.0 clutter-1.0 clutter-gst-1.0 gjs-gi-1.0  gjs-1.0])
AC_SUBST(DAX_CFLAGS)
AC_SUBST(DAX_LIBS)

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
                           -Wno-missing-field-initializers
                           -Wdeclaration-after-statement -Wmissing-declarations
                           -Wmissing-prototypes -Wredundant-decls
                           -Wmissing-noreturn -Wshadow -Wpointer-arith
                           -Wcast-align -Wwrite-strings -Winline
                           -Wformat-nonliteral -Wformat-security -Wswitch-enum
                           -Wswitch-default -Winit-self -Wmissing-include-dirs
                           -Wundef -Waggregate-return -Wmissing-format-attribute
                           -Wnested-externs -Wunsafe-loop-optimizations
                           -Wpacked -Winvalid-pch -Wsync-nand"])
      ]
)

AC_SUBST(MAINTAINER_CFLAGS)

dnl === Enable debug level ====================================================

m4_define([debug_default],
          [m4_if(m4_eval(dax_version_minor % 2), [1], [yes], [minimum])])

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
          DAX_DEBUG_CFLAGS="-DDAX_ENABLE_DEBUG"
        ],

        [minimum],
        [DAX_DEBUG_CFLAGS="-DDAX_ENABLE_DEBUG -DG_DISABLE_CAST_CHECKS"],

        [no],
        [DAX_DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"],

        [AC_MSG_ERROR([Unknown argument for --enable-debug])]
)

AC_SUBST(DAX_DEBUG_CFLAGS)

dnl === GObject-Introspection check ===========================================

GOBJECT_INTROSPECTION_CHECK([0.6.4])

AC_CONFIG_FILES([
    Makefile
    build/Makefile
    build/autotools/Makefile
    dax/Makefile
    tests/Makefile
    tools/Makefile
])

AC_OUTPUT

echo ""
echo "                     Dax $VERSION"
echo "                ==================="
echo ""
echo "                 prefix:   ${prefix}"
echo "         Compiler flags:   ${MAINTAINER_CFLAGS} ${CPPFLAGS}"
echo ""
echo "            debug level:   ${enable_debug}"
echo "  GObject introspection:   ${enable_introspection}"
echo ""
