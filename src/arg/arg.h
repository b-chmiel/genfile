#ifndef ARG_H
#define ARG_H

typedef enum { ASCII, NON_ASCII } generator_type_t;
typedef enum { GENERATE, VERIFY } arguments_type_t;

struct arguments_generate {
  struct arguments_global *global;

  char *filename;
  int seed;
  char *size;
  generator_type_t type;
};

struct arguments_verify {
  struct arguments_global *global;

  int block_size;
  char *file1;
  char *file2;
};

struct arguments_global {
  int verbosity;
  arguments_type_t type;
};

union arguments {
  struct arguments_generate generate;
  struct arguments_verify verify;
};

extern union arguments arg_arguments;

void parse(int argc, char *argv[]);

#endif
