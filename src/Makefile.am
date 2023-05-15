GITVERSION = $(shell git rev-list --max-count=1 HEAD || true)
AM_CFLAGS = -DGIT_VERSION=\"$(GITVERSION)\"
LDADD = $(LIB_ARGP_STANDALONE)

bin_PROGRAMS = gen_file
gen_file_SOURCES = main.c gen.c arg/arg.c arg/arg_generate.c arg/arg_verify.c
EXTRA_DIST = arg/arg.h arg/arg_generate.h arg/arg_verify.h gen.h bool.h