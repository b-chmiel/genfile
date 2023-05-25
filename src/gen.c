#include "gen.h"
#include "arg.h"
#include "bool.h"
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
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

static uint64_t parse_size(const char *size) {
  if (!have_postfix(size)) {
    return (uint64_t)atoi(size);
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

  uint64_t result;
  if (last == 'G') {
    result = (uint64_t)atoi(size_num) * (uint64_t)pow(1024, 3);
    free(size_num);
  } else if (last == 'M') {
    result = (uint64_t)atoi(size_num) * (uint64_t)pow(1024, 2);
    free(size_num);
  } else {
    fprintf(stderr, "G or M postfix required: %s\n", size_num);
    free(size_num);
    exit(EXIT_FAILURE);
  }

  return result;
}

static inline char generate(generator_type_t type) {
  if (type == NON_ASCII)
    return rand();

  return 'A' + (rand() % 26);
}

static int write_char_by_char(uint64_t size_bytes, FILE *file,
                              generator_type_t type) {
  fprintf(stderr, "Writing char by char with size_bytes = %ld\n", size_bytes);

  for (uint64_t i = 0; i < size_bytes; i++) {
    const char c = generate(type);
    if (fwrite(&c, 1, sizeof(c), file) != 1) {
      fprintf(stderr, "%s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

static int write_chunks(uint64_t chunk_size, uint64_t size_bytes, FILE *file,
                        generator_type_t type) {
  const uint64_t total_chunks = size_bytes / chunk_size;
  const uint64_t total_bytes = total_chunks * chunk_size;
  fprintf(stderr,
          "Writing chunk by chunk with chunk_size = %ld, num_of_chunks = %ld, "
          "total_bytes = %ld\n",
          chunk_size, total_chunks, total_bytes);

  unsigned char buffer[chunk_size + 1];
  for (uint64_t i = 0; i < total_chunks; ++i) {
    for (uint64_t j = 0; j < chunk_size; ++j) {
      buffer[j] = generate(type);
    }
    buffer[chunk_size] = '\0';

    if (fwrite(buffer, chunk_size, 1, file) != 1) {
      fprintf(stderr, "%s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int genfile(const struct arguments *arguments) {
  const uint64_t size_bytes = parse_size(arguments->size);
  fprintf(stderr, "Writing file with size %ld bytes\n", size_bytes);

  srand(arguments->seed);

  FILE *file = fopen(arguments->filename, "w+");
  if (file == NULL) {
    fprintf(stderr, "Could not create file '%s', %s\n", arguments->filename,
            strerror(errno));
    return EXIT_FAILURE;
  }

  const uint64_t chunk_size = sizeof(char) * pow(1024, 2); // 1 megabyte

  int ret = EXIT_FAILURE;
  if (chunk_size > size_bytes) {
    ret = write_char_by_char(size_bytes, file, arguments->type);
  } else {
    ret = write_chunks(chunk_size, size_bytes, file, arguments->type);

    const uint64_t remainder = size_bytes % chunk_size;
    if (remainder != 0) {
      ret = write_char_by_char(remainder, file, arguments->type);
    }
  }

  fclose(file);

  return ret;
}
