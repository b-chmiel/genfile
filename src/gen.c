#include "gen.h"
#include "arg.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *substring(const char *str, size_t start, size_t end) {
  const size_t length = end - start;
  char *result = malloc(sizeof(char) * sizeof(length + 1));
  strncpy(result, str + start, length);
  result[length] = '\0';

  return result;
}

static long parse_size(const char *size) {
  long result;
  const size_t input_length = strlen(size);

  const char last = size[input_length - 1];

  char *size_num = substring(size, 0, strlen(size) - 1);

  for (size_t i = 0; i < strlen(size_num); ++i) {
    if (!isdigit(size_num[i])) {
      fprintf(stderr, "Number required before postfix: %s\n", size_num);
      free(size_num);
      exit(EXIT_FAILURE);
    }
  }

  if (last == 'G') {
    result = (long int)atoi(size_num) * (long int)pow(1024, 3);
    free(size_num);
  } else if (last == 'M') {
    result = (long int)atoi(size_num) * (long int)pow(1024, 2);
    free(size_num);
  } else {
    fprintf(stderr, "G or M postfix required: %s\n", size_num);
    free(size_num);
    exit(EXIT_FAILURE);
  }

  return result;
}

int gen_file(const struct arguments *arguments) {
  const long size_bytes = parse_size(arguments->size);
  srand(arguments->seed);

  FILE *file = fopen(arguments->filename, "w+");

  const size_t chunk_size =
      sizeof(char) * pow(1024, 2) + 1; // 1 megabyte + 1 null terminator

  unsigned char buffer[chunk_size];
  for (long i = 0; i < size_bytes; i += chunk_size) {
    for (size_t j = 0; j < chunk_size - 1; ++j) {
      buffer[j] = rand();
    }
    buffer[chunk_size - 1] = '\0';

    if (fwrite(buffer, chunk_size, 1, file) != 1) {
      fprintf(stderr, "No Space Left on Device\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}