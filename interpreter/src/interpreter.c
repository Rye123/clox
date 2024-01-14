#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sysexits.h>
#include "token.h"
#include "interpreterState.h"
#define MAX_INPUT_LEN 255

// Runs a file, returning the exit code.
int runFile(char* filepath)
{
  FILE* fp;
  int exit_code = EX_OK;
  fp = fopen(filepath, "r");

  if (fp == NULL) {
    printf("Error: Could not open file %s.\n", filepath);
    exit(66); // EX_NOINPUT: Input file did not exist or was not readable.
  }

  // Get size of file
  fseek(fp, 0L, SEEK_END);
  size_t sz = ftell(fp);
  rewind(fp);

  // Allocate buffer
  char* buf = malloc(sz * sizeof(char));
  fread(buf, sz, 1, fp);
  fclose(fp);

  // Run source code
  State* state = StateNew();
  StateRun(state, buf);
  if (state->hadError)
    exit_code = EX_DATAERR;
  StateDelete(state);

  // Free buffers
  free(buf);
  return exit_code;
}

// Runs an interactive prompt, returning the exit code.
int runPrompt()
{
  char line[MAX_INPUT_LEN];
  State* state = StateNew();

  while (true) {
    printf("> ");
    if (fgets(line, sizeof(line), stdin)) {
      StateRun(state, line);
    } else {
      break; // No more input
    }

    state->hadError = false;  // Reset flag
  }
  StateDelete(state);
  return EX_OK;
}

int main(int argc, char** argv)
{
  int exit_code = EX_OK;
  if (argc > 2) {
    printf("Usage: interpreter [script]\n");
    exit(EX_USAGE);
  } else if (argc == 2) {
    exit_code = runFile(argv[1]);
  } else {
    exit_code = runPrompt();
  }
  return exit_code;
}
