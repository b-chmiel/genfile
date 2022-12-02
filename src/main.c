#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[]) {
  struct arguments arguments = {
      .filename = "file", .seed = 420, .size = "1G", .verbose = 1};

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.verbose) {
    printf("filename = %s\n", arguments.filename);
    printf("seed = %d\n", arguments.seed);
    printf("size = %s\n", arguments.size);
    printf("verbose = %d\n", arguments.verbose);
  }

  return EXIT_SUCCESS;
}
