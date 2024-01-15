#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"

Token* TokenNew(TokenType type, const char* lexeme, int lineNum)
{
  Token* ret = malloc(sizeof(Token));
  if (type == TOKEN_NUMBER) {
    // Convert lexeme into an int
    int literal_int = atoi(lexeme);
    Token tok = {type, lexeme, .literal.literal_int=literal_int, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else if (type == TOKEN_STRING) {
    Token tok = {type, lexeme, .literal.literal_str=lexeme, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else {
    Token tok = {type, lexeme, .literal.literal_null = NULL, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  }
  return ret;
}

void TokenDelete(Token* token)
{
  free(token);
}
