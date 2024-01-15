#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "token.h"

Token* TokenNew(TokenType type, const char* lexeme, const int lexemeLen, int lineNum)
{
  Token* ret = malloc(sizeof(Token));
  if (type == TOKEN_NUMBER) {
    // Convert lexeme into an string first for strtod
    char* string_literal = malloc((lexemeLen + 1) * sizeof(char));
    memcpy(string_literal, lexeme, lexemeLen);
    string_literal[lexemeLen] = '\0';

    // Convert from string to double
    double literal_num = strtod(string_literal, NULL);
    free(string_literal);
    Token tok = {type, lexeme, .literal.literal_num=literal_num, lineNum};
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
