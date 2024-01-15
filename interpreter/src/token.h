#ifndef _TOKEN_H_
#define _TOKEN_H_

typedef enum {
  // Single-character tokens
  TOKEN_PAREN_L, TOKEN_PAREN_R,
  TOKEN_BRACE_L, TOKEN_BRACE_R,
  TOKEN_COMMA, TOKEN_PERIOD,
  TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

  // One or more character tokens
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,

  // Literals
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

  // Keywords
  TOKEN_AND, TOKEN_OR,
  TOKEN_IF, TOKEN_ELSE, TOKEN_WHILE, TOKEN_FOR,
  TOKEN_TRUE, TOKEN_FALSE,
  TOKEN_CLASS, TOKEN_THIS, TOKEN_FUNC, TOKEN_RETURN, TOKEN_SUPER,
  TOKEN_NIL, TOKEN_VAR,
  TOKEN_PRINT,

  TOKEN_EOF
} TokenType;

typedef struct {
  const TokenType type;
  const char* lexeme;
  const union
  {
    void* literal_null;
    int literal_int;
    char* literal_str;
  } literal;
  const int lineNum;
} Token;

// Generates a new token. If the token type is a literal, the literal will be automatically generated.
Token* TokenNew(TokenType type, const char* lexeme, const int lexemeLength, int lineNum);

// Frees the memory associated with the token
void TokenDelete(Token* token);

#endif
