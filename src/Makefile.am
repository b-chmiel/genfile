GITVERSION = $(shell git rev-list --max-count=1 HEAD)
AM_CFLAGS = -DGIT_VERSION=\"$(GITVERSION)\"

bin_PROGRAMS = gen_file
gen_file_SOURCES = main.c gen.c arg.c
EXTRA_DIST = arg.h gen.h