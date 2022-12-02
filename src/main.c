#include <argp.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *argp_program_version = "gen_file 1.0";
const char *argp_program_bug_address = "bachm44@gmail.com";

struct arguments {
  char *filename;
  int verbose;
  int seed;
  char *size;
};

static struct argp_option options[] = {
    {"verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Verbose output"},
    {"seed", 'z', "SEED", OPTION_ARG_OPTIONAL,
     "Integer used for generating random file content"},
    {"size", 's', "SIZE", OPTION_ARG_OPTIONAL, "File size"},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 'z':
    if (arg == NULL) {
      argp_usage(state);
    }

    arguments->seed = atoi(arg);
    break;
  case 's':
    arguments->size = arg;
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 4) {
      argp_usage(state);
    }
    arguments->filename = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      argp_usage(state);
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return EXIT_SUCCESS;
}

static const char args_doc[] = "FILENAME";

static const char doc[] =
    "gen_file -- A program to generate random file with fixed seed ";

static struct argp argp = {options, parse_opt, args_doc, doc};

char *substring(const char *str, size_t start, size_t end) {
  char *result = malloc(sizeof(char) * sizeof(end - start + 1));
  strncpy(result, str + start, end - start);
  return result;
}

long parse_size(const char *size) {
  long result;
  char last = size[strlen(size) - 1];

  if (last == 'G') {
    char *size_num = substring(size, 0, strlen(size) - 1);
    result = (long int)atoi(size_num) * (long int)1073741824;
    free(size_num);
  } else if (last == 'M') {
    char *size_num = substring(size, 0, strlen(size) - 1);
    result = (long int)atoi(size_num) * (long int)1048576;
    free(size_num);
  }

  return result;
}

int gen_file(const struct arguments *arguments) {
  const int size_bytes = parse_size(arguments->size);
  srand(arguments->seed);

  int file =
      open(arguments->filename, O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG);

  const int chunk_size =
      sizeof(char) * 1024 * 1024 + 1; // 1 megabyte + 1 null terminator

  unsigned char buffer[chunk_size];
  for (int i = 0; i < size_bytes; i += chunk_size) {
    for (int j = 0; j < chunk_size - 1; ++j) {
      buffer[j] = rand();
    }
    buffer[chunk_size - 1] = '\0';

    write(file, buffer, chunk_size - 1);
  }

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  struct arguments arguments = {
      .filename = "file", .seed = 420, .size = "1G", .verbose = 0};

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.verbose) {
    printf("filename = %s\n", arguments.filename);
    printf("seed = %d\n", arguments.seed);
    printf("size = %s\n", arguments.size);
    printf("verbose = %d\n", arguments.verbose);
  }

  return gen_file(&arguments);
}
