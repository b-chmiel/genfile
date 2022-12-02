#ifndef ARG_H
#define ARG_H

struct arguments {
  char *filename;
  int verbose;
  int seed;
  char *size;
};

void parse(int argc, char *argv[], struct arguments *);

#endif
