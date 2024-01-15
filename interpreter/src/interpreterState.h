#ifndef _INTERPRETERSTATE_H_
#define _INTERPRETERSTATE_H_
#include "error.h"
#include "scanner.h"

// The current state of the interpreter
typedef struct State {
  bool hadError;
  Scanner* scanner;
} State;

// Create a new pointer to a State
State* StateNew();

// Runs the given source code in the state.
void StateRun(State* state, const char* source, const int sourceLen);

// Clears the memory associated with the state.
void StateDelete(State* state);

// Report an error associated with `lineNum` with `msg`.
void StateReportError(State* state, Error* err);

#endif
