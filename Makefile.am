AM_CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c99 -O3 -ffast-math $(MFLAGS)

AM_CPPFLAGS = $(VapourSynth_CFLAGS)

lib_LTLIBRARIES = libtonemap.la
libtonemap_la_SOURCES = tonemap.c
libtonemap_la_LDFLAGS = -no-undefined -avoid-version $(PLUGINLDFLAGS)
