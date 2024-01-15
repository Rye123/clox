#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "error.h"
#include "token.h"
#include "linkedlist.h"

Scanner* ScannerNew(const char* source, const int length)
{
  Scanner* scanner = malloc(sizeof(Scanner));
  Scanner tempScanner = {source, length, LinkedListNew(sizeof(Token)), LinkedListNew(sizeof(Error)), 0, 0, 0};
  memcpy(scanner, &tempScanner, sizeof(Scanner));

  return scanner;
}

// Add a token to the tokens list
void addToken(Scanner* scanner, TokenType type, char* lexeme)
{
  Token* newToken = TokenNew(type, lexeme, scanner->curLine);
  LinkedListAppend(scanner->tokens, newToken);
}

// Add an error to the errors list
void addError(Scanner* scanner, const char* msg)
{
  Error* newError = ErrorNew(scanner->curLine, scanner->curIdx, msg);
  LinkedListAppend(scanner->errors, newError);
}

bool reachedEnd(Scanner* scanner)
{
  return scanner->curIdx >= scanner->length;
}

// Advances the scanner, while returning the previous character (the one curIdx was on before advancement)
char advance(Scanner* scanner)
{
  char ret = *(scanner->source + scanner->curIdx);
  scanner->curIdx++;
  return ret;
}

// If the next character matches expected, advance the scanner and return true.
// Otherwise, returns false.
bool advanceCond(Scanner* scanner, const char expected)
{
  if (reachedEnd(scanner)) return false;

  char ret = *(scanner->source + scanner->curIdx);
  if (ret != expected) return false;

  scanner->curIdx++;
  return true;
}

// Scans until the start of the next lexeme, initialising the lexeme as a new token.
void scanToken(Scanner* scanner) {
  char c = advance(scanner);
  switch (c) {
  case '(': addToken(scanner, TOKEN_PAREN_L, NULL); break;
  case ')': addToken(scanner, TOKEN_PAREN_R, NULL); break;
  case '{': addToken(scanner, TOKEN_BRACE_L, NULL); break;
  case '}': addToken(scanner, TOKEN_BRACE_R, NULL); break;
  case ',': addToken(scanner, TOKEN_COMMA, NULL); break;
  case '.': addToken(scanner, TOKEN_PERIOD, NULL); break;
  case '-': addToken(scanner, TOKEN_MINUS, NULL); break;
  case '+': addToken(scanner, TOKEN_PLUS, NULL); break;
  case ';': addToken(scanner, TOKEN_SEMICOLON, NULL); break;
  case '*': addToken(scanner, TOKEN_STAR, NULL); break;

  case '!': addToken(scanner, advanceCond(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG, NULL); break;
  case '=': addToken(scanner, advanceCond(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL, NULL); break;
  case '<': addToken(scanner, advanceCond(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS, NULL); break;
  case '>': addToken(scanner, advanceCond(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER, NULL); break;
  default:
    addError(scanner, "Unexpected character.");
  }
}

void ScannerScan(Scanner* scanner)
{
  while (!reachedEnd(scanner)) {
    scanner->startIdx = scanner->curIdx;
    scanToken(scanner);
  }

  // Add EOF
  Token* eofToken = TokenNew(TOKEN_EOF, "", scanner->curLine);
  LinkedListAppend(scanner->tokens, eofToken);
}

void ScannerDelete(Scanner* scanner)
{
  LinkedListDelete(scanner->tokens);
  free(scanner);
}
