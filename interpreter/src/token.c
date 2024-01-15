#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "token.h"

Token* TokenNew(TokenType type, const char* lexeme, const int lexemeLen, int lineNum)
{
  Token* ret = malloc(sizeof(Token));

  // Create a null-terminated copy of the lexeme to be saved
  char* lexeme_cpy = NULL;
  if (lexeme != NULL) {
    lexeme_cpy = malloc((lexemeLen + 1) * sizeof(char));
    memcpy(lexeme_cpy, lexeme, lexemeLen);
    lexeme_cpy[lexemeLen] = '\0';
  }

  if (type == TOKEN_NUMBER) {
    // Convert from string to double
    double literal_num = strtod(lexeme_cpy, NULL);
    Token tok = {type, lexeme_cpy, .literal.literal_num=literal_num, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else if (type == TOKEN_STRING) {
    Token tok = {type, lexeme_cpy, .literal.literal_str = lexeme_cpy, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  } else {
    Token tok = {type, lexeme_cpy, .literal.literal_null=NULL, lineNum};
    memcpy(ret, &tok, sizeof(Token));
  }
  return ret;
}

// Returns true if `test` is an exact match for `word`. `word` should be a null-terminated string.
bool exactMatch(const char* word, const char* test, const int testLen)
{
  return testLen == strlen(word) && (strncmp(word, test, testLen) == 0);
}

TokenType TokenMatchKeyword(const char* lexeme, const int lexemeLength)
{
  if (exactMatch("and", lexeme, lexemeLength)) return TOKEN_AND;
  else if (exactMatch("or", lexeme, lexemeLength)) return TOKEN_OR;
  else if (exactMatch("if", lexeme, lexemeLength)) return TOKEN_IF;
  else if (exactMatch("else", lexeme, lexemeLength)) return TOKEN_ELSE;
  else if (exactMatch("while", lexeme, lexemeLength)) return TOKEN_WHILE;
  else if (exactMatch("for", lexeme, lexemeLength)) return TOKEN_FOR;
  else if (exactMatch("true", lexeme, lexemeLength)) return TOKEN_TRUE;
  else if (exactMatch("false", lexeme, lexemeLength)) return TOKEN_FALSE;
  else if (exactMatch("class", lexeme, lexemeLength)) return TOKEN_CLASS;
  else if (exactMatch("this", lexeme, lexemeLength)) return TOKEN_THIS;
  else if (exactMatch("func", lexeme, lexemeLength)) return TOKEN_FUNC;
  else if (exactMatch("return", lexeme, lexemeLength)) return TOKEN_RETURN;
  else if (exactMatch("super", lexeme, lexemeLength)) return TOKEN_SUPER;
  else if (exactMatch("nil", lexeme, lexemeLength)) return TOKEN_NIL;
  else if (exactMatch("var", lexeme, lexemeLength)) return TOKEN_VAR;
  else if (exactMatch("print", lexeme, lexemeLength)) return TOKEN_PRINT;
  return TOKEN_IDENTIFIER;
}

void TokenDelete(Token* token)
{
  free(token->lexeme);
  free(token);
}
