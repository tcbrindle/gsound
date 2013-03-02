
NULL=

INCLUDES = \
    -I$(top_srcdir)/gsound \
    $(GSOUND_CFLAGS) \
    $(NULL)

noinst_PROGRAMS = \
    test-c \
    test-vala \
    $(NULL)

test_c_SOURCES = \
    test-c.c \
    $(NULL)

test_c_LDADD = \
    $(top_builddir)/gsound/libgsound.la \
    $(GSOUND_LIBS) \
    $(NULL)

test_vala_SOURCES = \
    test-vala.vala \
    $(NULL)

test_vala_VALAFLAGS = \
    --vapidir=$(top_builddir)/gsound \
    --pkg gsound-1.0 \
    $(NULL)

test_vala_LDADD = $(test_c_LDADD)

EXTRA_DIST = \
    test-gjs.js \
    test-py.py