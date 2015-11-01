## Process this file with automake to produce Makefile.in

NULL =

lib_LTLIBRARIES = libgsound.la

libgsound_la_SOURCES = \
	gsound-context.c gsound-context.h gsound-attr.h \
	$(NULL)

libgsound_la_CPPFLAGS = \
	${GSOUND_CFLAGS} \
	${NULL}

libgsound_la_CFLAGS =\
	${WARN_CFLAGS} \
	${NULL}

libgsound_la_LDFLAGS = \
	-version-info $(GSOUND_LT_VERSION) \
	${WARN_LDFLAGS} \
	$(NULL)

libgsound_la_LIBADD = \
	$(GSOUND_LIBS) \
	${NULL}

include_HEADERS = \
	gsound.h \
	gsound-attr.h \
	gsound-context.h \
	${NULL}

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gsound.pc

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = \
    --add-include-path=$(srcdir) \
    --symbol-prefix=gsound \
    --warn-all -v
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = $(libgsound_la_SOURCES)

GSound-1.0.gir: libgsound.la
GSound_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
GSound_1_0_gir_CFLAGS = $(INCLUDES) ${WARN_SCANNERFLAGS}
GSound_1_0_gir_LIBS = libgsound.la
GSound_1_0_gir_FILES = $(introspection_sources)
GSound_1_0_gir_SCANNERFLAGS = --c-include "gsound.h"
INTROSPECTION_GIRS += GSound-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)


if ENABLE_VAPIGEN
include $(VAPIGEN_MAKEFILE)

gsound.vapi: GSound-1.0.gir GSound-1.0.metadata gsound.deps

VAPIGEN_VAPIS = \
	gsound.vapi

gsound_vapi_DEPS = glib-2.0 gio-2.0
gsound_vapi_METADATADIRS = $(srcdir)
gsound_vapi_FILES = GSound-1.0.gir

VAPI_DEPS = $(VAPIGEN_VAPIS:.vapi=.deps)

gsound.deps: Makefile.am
	$(AM_V_GEN) echo $(gsound_vapi_DEPS) | tr ' ' '\n' > $@

vapidir = $(datadir)/vala/vapi
vapi_DATA = \
	$(VAPIGEN_VAPIS) \
	$(VAPI_DEPS)

CLEANFILES += \
	$(VAPIGEN_VAPIS) \
	$(VAPI_DEPS)

endif # ENABLE_VAPIGEN
endif # HAVE_INTROSPECTION

EXTRA_DIST = \
	gsound.pc.in \
	GSound-1.0.metadata

-include $(top_srcdir)/git.mk

