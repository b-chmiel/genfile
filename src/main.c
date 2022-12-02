#include <stdio.h>

#include "arg.h"
#include "gen.h"

int main(int argc, char *argv[]) {
  struct arguments arguments = {
      .filename = "file", .seed = 420, .size = "1G", .verbose = 0};

  parse(argc, argv, &arguments);

  if (arguments.verbose) {
    printf("filename = %s\n", arguments.filename);
    printf("seed = %d\n", arguments.seed);
    printf("size = %s\n", arguments.size);
    printf("verbose = %d\n", arguments.verbose);
  }

  return gen_file(&arguments);
}
