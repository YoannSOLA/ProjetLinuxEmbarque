MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure COPYING \
					   INSTALL config.h.in config

ACLOCAL_AMFLAGS = -I m4

maintainer-clean-local:
	-rm -rf config
	-rm -r m4

bin_PROGRAMS = src/client/client src/miniserver/miniserver
client_SOURCES = src/client/client.c
miniserver_SOURCES = src/miniserver/miniserver.c
