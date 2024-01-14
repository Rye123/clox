#ifndef _INTERPRETERSTATE_H_
#define _INTERPRETERSTATE_H_

// The current state of the interpreter
typedef struct State {
  bool hadError;
} State;

// Create a new pointer to a State
State* StateNew();

// Runs the given source code in the state.
void StateRun(State* state, const char* source);

// Clears the memory associated with the state.
void StateDelete(State* state);

// Report an error associated with `lineNum` with `msg`.
void StateReportError(State* state, int lineNum, const char* msg);










#endif
