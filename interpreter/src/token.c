#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"

Token* TokenNew(TokenType type, const char* lexeme, const int lexemeLen, int lineNum)
{
  Token* ret = malloc(sizeof(Token));
  if (type == TOKEN_NUMBER) {
    // Convert lexeme into an int
    int literal_int = atoi(lexeme);
    Token tok = {type, lexeme, .literal.literal_int=literal_int, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else if (type == TOKEN_STRING) {
    // Allocate space for the string
    char* string_literal = malloc((lexemeLen + 1) * sizeof(char));
    memcpy(string_literal, lexeme, lexemeLen);
    string_literal[lexemeLen] = '\0';
    
    Token tok = {type, lexeme, .literal.literal_str=string_literal, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else {
    Token tok = {type, lexeme, .literal.literal_null = NULL, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  }
  return ret;
}

void TokenDelete(Token* token)
{
  if (token->type == TOKEN_STRING) {
    // free space allocated for the string
    free(token->literal.literal_str);
  }
  free(token);
}
