
NULL = 

if ENABLE_VAPIGEN

bin_PROGRAMS = gsound-play

gsound_play_SOURCES = gsound-play.vala

gsound_play_VALAFLAGS = \
    --vapidir ${abs_top_builddir}/gsound \
    --pkg gsound \
    ${NULL}

gsound_play_CPPFLAGS = \
    -I${top_srcdir}/gsound \
    ${GSOUND_CFLAGS} \
    ${NULL}

gsound_play_LDFLAGS = \
    ${top_builddir}/gsound/libgsound.la \
    ${GSOUND_LIBS} \
    ${NULL}

endif # ENABLE_VAPIGEN

-include $(top_srcdir)/git.mk
