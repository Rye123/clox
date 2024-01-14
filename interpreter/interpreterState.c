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

void StateRun(State* state, char* source)
{
  printf("RUNNING: %s\n", source);
}


void StateDelete(State* state)
{
  free(state);
}
