#include "arg.h"
#include "arg_generate.h"
#include "arg_verify.h"
#include <argp.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGUMENT_MAX_SIZE 255
#ifndef GIT_VERSION
#error "GIT_VERSION NOT DEFINED"
#endif

#define SUBCOMMAND_VERIFY "verify"
#define SUBCOMMAND_GENERATE "generate"

const char *argp_program_version = "gen_file-1.0.5-dev-" GIT_VERSION;
const char *argp_program_bug_address = "bachm44@gmail.com";

static const struct argp_option options_global[] = {
    {"verbose", 'v', "level", OPTION_ARG_OPTIONAL, "Verbosity level", -1}, {0}};

static const char documentation_global[] =
    "\n"
    "Program for generating and verifying files"
    "\v"
    "Supported commands are:\n"
    "  generate    Generate file."
    "  verify    Compare generated files.";

static void log_printf(struct arguments_global *g, int level, const char *fmt,
                       ...) {
  va_list ap;
  FILE *f = stdout;

  if (g->verbosity < level)
    return;

  if (level == 0)
    f = stderr;

  va_start(ap, fmt);

  vfprintf(f, fmt, ap);

  va_end(ap);
}

static const char *argp_key(int key, char *keystr) {
  keystr[0] = key;
  keystr[1] = 0;

  switch (key) {
  case ARGP_KEY_ARG:
    return "ARGP_KEY_ARG";
  case ARGP_KEY_ARGS:
    return "ARGP_KEY_ARGS";
  case ARGP_KEY_END:
    return "ARGP_KEY_END";
  case ARGP_KEY_NO_ARGS:
    return "ARGP_KEY_NO_ARGS";
  case ARGP_KEY_INIT:
    return "ARGP_KEY_INIT";
  case ARGP_KEY_SUCCESS:
    return "ARGP_KEY_SUCCESS";
  case ARGP_KEY_ERROR:
    return "ARGP_KEY_ERROR";
  case ARGP_KEY_FINI:
    return "ARGP_KEY_FINI";
  }

  return keystr;
}

static error_t parse_global(int key, char *arg, struct argp_state *state) {
  struct arguments_global *global = state->input;
  char keystr[2];

  log_printf(global, 3, "x: parsing %s = '%s'\n", argp_key(key, keystr),
             arg ? arg : "(null)");

  switch (key) {
  case 'v':
    if (arg)
      global->verbosity = atoi(arg);
    else
      global->verbosity++;
    log_printf(global, 2, "x: set verbosity to %d\n", global->verbosity);
    break;

  case ARGP_KEY_ARG:
    assert(arg);
    if (strcmp(arg, SUBCOMMAND_GENERATE) == 0) {
      cmd_generate(state);
    } else if (strcmp(arg, SUBCOMMAND_VERIFY) == 0) {
      cmd_verify(state);
    } else {
      argp_error(state, "%s is not a valid command", arg);
    }
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {
    options_global,
    parse_global,
    "[<cmd> [CMD-OPTIONS]]...",
    documentation_global,
};

void parse(int argc, char *argv[]) {
  struct arguments_global global = {.verbosity = 1};

  log_printf(&global, 3, "x: begin (argc = %d, argv[0] = %s)\n", argc, argv[0]);

  argp_parse(&argp, argc, argv, ARGP_IN_ORDER, NULL, &global);
}
