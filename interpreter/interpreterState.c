#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "interpreterState.h"

State* StateNew()
{
  State* state = malloc(sizeof(State));
  state->hadError = false;
  return state;
}

void StateRun(State* state, const char* source)
{
  printf("RUNNING: %s\n", source);
}

void StateReportError(State* state, int lineNum, const char* msg)
{
  printf("[Line %d] Error: %s\n", lineNum, msg);
  state->hadError = true;
}


void StateDelete(State* state)
{
  free(state);
}
