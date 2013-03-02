## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = gsound docs test

gsounddocdir = ${prefix}/doc/gsound
gsounddoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(gsounddoc_DATA)

DISTCHECK_CONFIGURE_FLAGS = --enable-introspection --enable-gtk-doc


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gsounddocdir)
