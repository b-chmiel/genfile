#include "gen.h"
#include "arg.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *substring(const char *str, size_t start, size_t end) {
  char *result = malloc(sizeof(char) * sizeof(end - start + 1));
  strncpy(result, str + start, end - start);
  return result;
}

static long parse_size(const char *size) {
  long result;
  const size_t input_length = strlen(size);

  const char last = size[input_length - 1];

  if (last == 'G') {
    char *size_num = substring(size, 0, strlen(size) - 1);
    result = (long int)atoi(size_num) * (long int)pow(1024, 3);
    free(size_num);
  } else if (last == 'M') {
    char *size_num = substring(size, 0, strlen(size) - 1);
    result = (long int)atoi(size_num) * (long int)pow(1024, 2);
    free(size_num);
  }

  return result;
}

int gen_file(const struct arguments *arguments) {
  const int size_bytes = parse_size(arguments->size);
  srand(arguments->seed);

  FILE *file = fopen(arguments->filename, "w+");

  const int chunk_size =
      sizeof(char) * pow(1024, 2) + 1; // 1 megabyte + 1 null terminator

  unsigned char buffer[chunk_size];
  for (int i = 0; i < size_bytes; i += chunk_size) {
    for (int j = 0; j < chunk_size - 1; ++j) {
      buffer[j] = rand();
    }
    buffer[chunk_size - 1] = '\0';

    fwrite(buffer, chunk_size, 1, file);
  }

  return EXIT_SUCCESS;
}