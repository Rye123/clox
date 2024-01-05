#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_INPUT_LEN 255

void run(char* source)
{
  printf("INPUT: %s\n", source);
}

void runFile(char* filepath)
{
  FILE* fp;
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
  run(buf);

  // Free buffers
  free(buf);
}

void runPrompt()
{
  char line[MAX_INPUT_LEN];

  while (true) {
    printf("> ");
    if (fgets(line, sizeof(line), stdin)) {
      run(line);
    } else {
      return;
    }
  }
}

int main(int argc, char** argv)
{
  if (argc > 2) {
    printf("Usage: interpreter [script]\n");
    exit(64);  // EX_USAGE: Command used incorrectly
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return 0;
}
