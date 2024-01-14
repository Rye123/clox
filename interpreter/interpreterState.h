#ifndef _INTERPRETERSTATE_H_
#define _INTERPRETERSTATE_H_

// The current state of the interpreter
typedef struct State {
  bool hadError;
} State;

// Create a new pointer to a State
State* StateNew();

// Runs the given source code in the state.
void StateRun(State* state, char* source);

// Clears the memory associated with the state.
void StateDelete(State* state);

#endif
