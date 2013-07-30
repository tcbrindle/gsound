dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(gsound, 0.0.1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_MACRO_DIR(m4)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_PROG_CC
AM_PROG_VALAC

LT_INIT

PKG_CHECK_MODULES(GSOUND,
[
    gobject-2.0
    gio-2.0
    libcanberra
])
AC_SUBST(GSOUND_CFLAGS)
AC_SUBST(GSOUND_LIBS)

GTK_DOC_CHECK(1.14, [--flavour no-tmpl])

GOBJECT_INTROSPECTION_CHECK([1.2.9])

VALA_REQUIRED=0.17.2.12

VAPIGEN_CHECK($VALA_REQUIRED)

#if test "$enable_vala" != "no"; then
#	AC_PATH_PROG([VALAC], [valac], [])
#fi
#AM_CONDITIONAL(HAVE_VALAC, test "x$VALAC" != "x")


AC_OUTPUT([
Makefile
docs/Makefile
test/Makefile
gsound/gsound.pc
gsound/Makefile
])
