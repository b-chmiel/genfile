#include "gen.h"
#include "arg.h"
#include "bool.h"
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *substring(const char *str, size_t start, size_t end) {
  const size_t length = end - start;
  char *result = malloc(sizeof(char) * (length + 1));
  strncpy(result, str + start, length);
  result[length] = '\0';

  return result;
}

static bool_t have_postfix(const char *size) {
  for (size_t i = 0; i < strlen(size); ++i) {
    if (!isdigit(size[i])) {
      return TRUE;
    }
  }

  return FALSE;
}

static long parse_size(const char *size) {
  if (!have_postfix(size)) {
    return (long int)atoi(size);
  }

  char *size_num = substring(size, 0, strlen(size) - 1);

  for (size_t i = 0; i < strlen(size_num); ++i) {
    if (!isdigit(size_num[i])) {
      fprintf(stderr, "Number required before postfix: %s\n", size_num);
      free(size_num);
      exit(EXIT_FAILURE);
    }
  }

  const char last = size[strlen(size) - 1];

  long result;
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

static int write_char_by_char(long size_bytes, FILE *file) {
  fprintf(stderr, "Writing char by char with size_bytes = %ld\n", size_bytes);

  for (long i = 0; i < size_bytes; i++) {
    const char c = rand();
    if (fwrite(&c, 1, sizeof(c), file) != 1) {
      fprintf(stderr, "%s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

static int write_chunks(size_t chunk_size, long size_bytes, FILE *file) {
  fprintf(stderr,
          "Writing chunk by chunk with chunk_size = %ld, size_bytes = %ld\n",
          chunk_size, size_bytes);

  unsigned char buffer[chunk_size];
  for (long i = 0; i < size_bytes; i += chunk_size) {
    for (size_t j = 0; j < chunk_size - 1; ++j) {
      buffer[j] = rand();
    }
    buffer[chunk_size - 1] = '\0';

    if (fwrite(buffer, chunk_size, 1, file) != 1) {
      fprintf(stderr, "%s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int gen_file(const struct arguments *arguments) {
  const long size_bytes = parse_size(arguments->size);
  srand(arguments->seed);

  FILE *file = fopen(arguments->filename, "w+");
  if (file == NULL) {
    fprintf(stderr, "Could not create file '%s', %s\n", arguments->filename,
            strerror(errno));
    return EXIT_FAILURE;
  }

  const size_t chunk_size =
      sizeof(char) * pow(1024, 2) + 1; // 1 megabyte + 1 null terminator

  int ret = EXIT_FAILURE;
  if (chunk_size > (size_t)size_bytes) {
    ret = write_char_by_char(size_bytes, file);
  } else {
    ret = write_chunks(chunk_size, size_bytes, file);
  }

  fclose(file);

  return ret;
}
