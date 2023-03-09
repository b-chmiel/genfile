GITVERSION = $(shell git rev-list --max-count=1 HEAD || true)
AM_CFLAGS = -DGIT_VERSION=\"$(GITVERSION)\"
LDADD = $(LIB_ARGP_STANDALONE)

bin_PROGRAMS = gen_file
gen_file_SOURCES = main.c gen.c arg.c
EXTRA_DIST = arg.h gen.h