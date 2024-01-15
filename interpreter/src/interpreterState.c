#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "error.h"
#include "scanner.h"
#include "token.h"
#include "interpreterState.h"

State* StateNew()
{
  State* state = malloc(sizeof(State));
  state->hadError = false;
  state->scanner = NULL;
  return state;
}

void StateRun(State* state, const char* source, const int sourceLen)
{
  printf("Running: %s\n", source);
  state->scanner = ScannerNew(source, sourceLen);
  ScannerScan(state->scanner);

  // Loop through errors if any
  int errCount = LinkedListLength(state->scanner->errors);
  if (errCount > 0) {
    LLNode* node = state->scanner->errors->head;
    for (int i = 0; i < errCount; i++) {
      Error* err = (Error*) node->data;
      StateReportError(state, err);
      node = node->next;
    }
  }

  // Output tokens
  int tokCount = LinkedListLength(state->scanner->tokens);
  if (tokCount > 0) {
    LLNode* node = state->scanner->tokens->head;
    for (int i = 0; i < tokCount; i++) {
      Token* tok = (Token*) node->data;
      printf("Token %d (%s)", i, TokenTypeString[tok->type]);
      if (tok->type == TOKEN_STRING)
	printf(": %s\n", tok->literal.literal_str);
      else if (tok->type == TOKEN_NUMBER)
	printf(": %f\n", tok->literal.literal_num);
      else if (tok->type == TOKEN_IDENTIFIER)
	printf(": %s\n", tok->lexeme);
      else
	printf("\n");
      node = node->next;
    }
  }
}

void StateReportError(State* state, Error* err)
{
  printf("[Line %d] Error at %d: %s\n", err->line+1, err->srcIndex, err->errMsg);
  state->hadError = true;
}


void StateDelete(State* state)
{
  if (state->scanner != NULL) {
    ScannerDelete(state->scanner);
  }
  free(state);
}
