#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
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

bool isDigit(char c)
{
  return (c >= '0' && c <= '9');
}

bool isAlpha(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool isAlnum(char c)
{
  return isAlpha(c) || isDigit(c);
}

// Add a token to the tokens list with a manual size.
void addTokenWithSize(Scanner* scanner, TokenType type, const char* lexeme, int lexemeLen)
{
  Token* newToken = TokenNew(type, lexeme, lexemeLen, scanner->curLine);
  LinkedListAppend(scanner->tokens, newToken);
}

// Add a token to the tokens list, automatically setting the size based on startIdx and curIdx.
void addToken(Scanner* scanner, TokenType type, const char* lexeme)
{
  size_t lexemeLen = scanner->curIdx - scanner->startIdx;
  addTokenWithSize(scanner, type, lexeme, lexemeLen);
}

// Add an error to the errors list
void addError(Scanner* scanner, const char* msg)
{
  Error* newError = ErrorNew(scanner->curLine, scanner->curIdx-1, msg);  // -1, because the error occurs at the previously consumed character.
  LinkedListAppend(scanner->errors, newError);
}

bool reachedEnd(Scanner* scanner)
{
  return scanner->curIdx >= scanner->length;
}

// Returns the current character. This is basically advance(), without actually advancing.
char peek(Scanner *scanner)
{
  return *(scanner->source + scanner->curIdx);
}

// Returns the next character.
char peekNext(Scanner *scanner)
{
  if (scanner->curIdx + 1 >= scanner->length) return '\0';
  return *(scanner->source + scanner->curIdx + 1);
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
  case '/':
    if (advanceCond(scanner, '/')) {
      // This is a comment, ignore the rest of the line
      while (peek(scanner) != '\n' && !reachedEnd(scanner)) advance(scanner);
    } else {
      addToken(scanner, TOKEN_SLASH, NULL);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    scanner->curLine++; break;
  case '"': {
    // Handle string
    while (peek(scanner) != '"' && !reachedEnd(scanner)) {
      // We've yet to reach the end of the string
      if (peek(scanner) == '\n') scanner->curLine++;
      advance(scanner);
    }

    if (reachedEnd(scanner)) {
      addError(scanner, "Unterminated string.");
      return;
    }

    // Here, we've reached the end of the string without any issues
    advance(scanner); // to include the closing "

    // Add the string without the enclosing quotes
    int strStartIdx = scanner->startIdx + 1;
    int strEndIdx = scanner->curIdx - 1;
    addTokenWithSize(scanner, TOKEN_STRING, (scanner->source + strStartIdx), strEndIdx - strStartIdx);
    break;
  }
    
  default: {
    if (isDigit(c)) {
      // Handle number
      while (isDigit(peek(scanner))) advance(scanner);

      // If it has fractional part...
      if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
	advance(scanner); // Consume the '.'
	while (isDigit(peek(scanner))) advance(scanner);
      }

      addToken(scanner, TOKEN_NUMBER, (scanner->source + scanner->startIdx));
      if (errno == ERANGE) {
        // Remove token and report error
        Token *tok = LinkedListPop(scanner->tokens);
        TokenDelete(tok);
        addError(scanner, "Number exceeds range.");
      }

    } else if (isAlpha(c)) {
      // Handle possible identifier or reserved keyword
      while (isAlnum(peek(scanner))) advance(scanner);

      TokenType type = TokenMatchKeyword((scanner->source + scanner->startIdx), (scanner->curIdx - scanner->startIdx));
      addToken(scanner, type, (scanner->source + scanner->startIdx));
    } else {
      addError(scanner, "Unexpected character.");
    }
  }
  }
}

void ScannerScan(Scanner* scanner)
{
  while (!reachedEnd(scanner)) {
    scanner->startIdx = scanner->curIdx;
    scanToken(scanner);
  }

  // Add EOF
  Token* eofToken = TokenNew(TOKEN_EOF, "", 0, scanner->curLine);
  LinkedListAppend(scanner->tokens, eofToken);
}

void ScannerDelete(Scanner* scanner)
{
  // Free memory associated with all lists
  while (LinkedListLength(scanner->tokens) > 0) {
    Token* token = (Token*) LinkedListPop(scanner->tokens);
    TokenDelete(token);
  }
  while (LinkedListLength(scanner->errors) > 0) {
    Error* error = (Error*) LinkedListPop(scanner->errors);
    ErrorDelete(error);
  }

  assert (LinkedListLength(scanner->tokens) == 0);
  assert (LinkedListLength(scanner->errors) == 0);
  
  // Delete lists
  LinkedListDelete(scanner->tokens);
  LinkedListDelete(scanner->errors);
  free(scanner);
}
