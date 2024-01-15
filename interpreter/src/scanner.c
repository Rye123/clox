#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "linkedlist.h"

Scanner* ScannerNew(const char* source, const int length)
{
  Scanner* scanner = malloc(sizeof(Scanner));
  Scanner tempScanner = {source, length, LinkedListNew(sizeof(Token)), 0, 0, 0};
  memcpy(scanner, &tempScanner, sizeof(Scanner));

  return scanner;
}

bool reachedEnd(Scanner* scanner) {
  return scanner->curIdx >= scanner->length;
}

char advance(Scanner* scanner) {
  char ret = *(scanner->source + scanner->curIdx);
  scanner->curIdx++;
  return ret;
}

void addToken(Scanner* scanner, TokenType type, char* lexeme)
{
  Token* newToken = TokenNew(type, lexeme, scanner->curLine);
  LinkedListAppend(scanner->tokens, newToken);
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
