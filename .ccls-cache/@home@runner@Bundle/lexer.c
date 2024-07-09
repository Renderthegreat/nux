#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

static char *keywords[] = {"int", "void", "class"};
static TokenType keywordTokens[] = {TOKEN_INT, TOKEN_VOID, TOKEN_CLASS};

Token *createToken(TokenType type, const char *value) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->type = type;
  token->value = strdup(value);
  return token;
}

Token *tokenize(const char *source) {
  const char *ptr = source;
  Token *tokens =
      (Token *)malloc(sizeof(Token) * 100); // Example token array size
  int tokenIndex = 0;

  while (*ptr != '\0') {
    if (isspace(*ptr)) {
      ptr++;
      continue;
    }

    // Keyword or identifier
    if (isalpha(*ptr)) {
      char buffer[MAX_TOKEN_LENGTH] = {0};
      int length = 0;
      while (isalnum(*ptr)) {
        buffer[length++] = *ptr++;
      }
      buffer[length] = '\0';

      TokenType type = TOKEN_IDENTIFIER;
      for (int i = 0; i < sizeof(keywords) / sizeof(char *); ++i) {
        if (strcmp(buffer, keywords[i]) == 0) {
          type = keywordTokens[i];
          break;
        }
      }

      tokens[tokenIndex++] = *createToken(type, buffer);
      continue;
    }

    // Number
    if (isdigit(*ptr)) {
      char buffer[MAX_TOKEN_LENGTH] = {0};
      int length = 0;
      while (isdigit(*ptr)) {
        buffer[length++] = *ptr++;
      }
      buffer[length] = '\0';
      tokens[tokenIndex++] = *createToken(TOKEN_NUMBER, buffer);
      continue;
    }

    // Operator or punctuation
    if (ispunct(*ptr)) {
      char buffer[2] = {*ptr, '\0'};
      TokenType type = TOKEN_UNKNOWN;

      if (strchr("+-*/=;", *ptr)) {
        type = TOKEN_OPERATOR;
      } else if (strchr("(){},;", *ptr)) {
        type = TOKEN_PUNCTUATION;
      }

      tokens[tokenIndex++] = *createToken(type, buffer);
      ptr++;
      continue;
    }
  }

  tokens[tokenIndex] = *createToken(TOKEN_EOF, "");
  return tokens;
}