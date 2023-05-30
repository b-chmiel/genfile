#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([genfile], [1.0.5], [])
AM_INIT_AUTOMAKE([-Wall -Werror foreign -Wno-portability])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

# Checks for programs.
AC_PROG_CC

AC_ARG_WITH([argp-standalone],
	AS_HELP_STRING([--with-argp-standalone], [compile with argp-standalone]),
	[], with_argp_standalone=no)

if test "${with_argp_standalone}" = "yes"; then
 AC_CHECK_LIB(argp, 
	argp_parse,
	[LIB_ARGP_STANDALONE="-largp"],
	AC_MSG_ERROR([argp-standalone selected but not found]))
fi

AC_SUBST([LIB_ARGP_STANDALONE])

AC_OUTPUT
