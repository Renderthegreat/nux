/*
 * Please read the main.c file for license.
 */

#ifndef LEXER_H
#define LEXER_H

typedef enum {
  TOKEN_LET,
  TOKEN_VOID,
  TOKEN_FUNCTION,
  TOKEN_PARENTHISES,
  TOKEN_BODY,
  TOKEN_TYPE,
  TOKEN_OBJECT,
  TOKEN_LIST,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_LITERAL,
  TOKEN_OPERATOR,
  TOKEN_SET,
  TOKEN_PUNCTUATION,
  TOKEN_DEBUG,
  TOKEN_RETURN,
  TOKEN_EOL,
  TOKEN_EOF,
  TOKEN_UNKNOWN
} TokenType;

typedef struct {
  TokenType type;
  char *value;
} Token;

Token *tokenize(const char *source);

#endif