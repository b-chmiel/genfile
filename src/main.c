#include <stdio.h>

#include "arg.h"
#include "gen.h"

int main(int argc, char *argv[]) {
  struct arguments arguments = {
      .filename = "file", .seed = 420, .size = "10M", .verbose = 0, .type = 0};

  parse(argc, argv, &arguments);

  if (arguments.verbose) {
    printf("filename = %s\n", arguments.filename);
    printf("seed = %d\n", arguments.seed);
    printf("size = %s\n", arguments.size);
    printf("verbose = %d\n", arguments.verbose);

    if (arguments.type) {
      printf("generation type = NON ASCII\n");
    } else {
      printf("generation type = ASCII\n");
    }
  }

  return gen_file(&arguments);
}
