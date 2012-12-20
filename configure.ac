#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_INIT([property-parser], [0.1], [g.lipari@sssup.it])
AM_INIT_AUTOMAKE([property_parser.cpp], 1.0)
AC_CONFIG_SRCDIR([property_parser.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)
AC_PROG_LIBTOOL
AC_CHECK_PROGS(RANLIB, ranlib)

# Checks for libraries.


# Checks for header files.

AC_CHECK_HEADERS([boost/spirit.hpp], [], 
                 [AC_MSG_ERROR(You need the Spirit Boost library)])
AC_CHECK_HEADERS([boost/variant.hpp], [], 
                 [AC_MSG_ERROR(You need the Spirit Variant library)])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_OUTPUT(Makefile, [date > stamp-h])
