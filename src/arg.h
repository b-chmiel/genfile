#ifndef ARG_H
#define ARG_H

typedef enum { ASCII, NON_ASCII } generator_type_t;

struct arguments {
  char *filename;
  int verbose;
  int seed;
  char *size;
  generator_type_t type;
};

void parse(int argc, char *argv[], struct arguments *);

#endif
