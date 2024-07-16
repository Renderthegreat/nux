/*
  * Please read the main.c file for license.
  */

#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 256

static char *keywords[] = {"let",   "void",   "func", "return", "if",
                                                      "debug", "return", "type", "object"};
static TokenType keywordTokens[] = {TOKEN_LET,    TOKEN_VOID, TOKEN_FUNCTION,
                                                                        TOKEN_RETURN, TOKEN_IF,   TOKEN_DEBUG,
                                                                        TOKEN_RETURN, TOKEN_TYPE, TOKEN_OBJECT};

Token *createToken(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    if (value == NULL) {
        token->value = NULL;
    } else {
        token->value = strdup(value);
    }
    return token;
}

char *remchar(const char *str, char ch) {
    int i = 0, j = 0;
    char *out = (char *)malloc(strlen(str) + 1);

    if (out == NULL) {
        return NULL;
    }

    while (str[i] != '\0') {
        if (str[i] != ch) {
            out[j++] = str[i];
        }
        i++;
    }
    out[j] = '\0';

    return out;
}

Token *tokenize(const char *source) {
    const char *ptr = source;
    Token *tokens = (Token *)malloc(sizeof(Token) * 100);
    int tokenIndex = 0;

    while (*ptr != '\0') {
        if (isspace(*ptr)) {
            ptr++;
            continue;
        }

        if (*ptr == ';') {
            tokens[tokenIndex++] = *createToken(TOKEN_EOL, ";");
            ptr++; // Move past the semicolon
            continue;
        }

        if (*ptr == '=') {
            tokens[tokenIndex++] = *createToken(TOKEN_SET, "=");
            ptr++;
            continue;
        }

        if (isalpha(*ptr) || *ptr == '_') {
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while ((isalnum(*ptr) || *ptr == '_') && *ptr != ';') {
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

        if (*ptr == '{') {
            ptr++;
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (*ptr != '}' && *ptr != '\0') {
                buffer[length++] = *ptr;
                ptr++;
            }
            buffer[length] = '\0';
            if (*ptr == '}') {
                ptr++; // Skip the closing bracket
            }
            tokens[tokenIndex++] = *createToken(TOKEN_BODY, buffer);
            continue;
        }

        if (*ptr == '\n' || *ptr == '\t') {
            ptr++;
            continue;
        }

        if (isdigit(*ptr)) {
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (isdigit(*ptr) && *ptr != ';') {
                buffer[length++] = *ptr++;
            }
            buffer[length] = '\0';
            tokens[tokenIndex++] = *createToken(TOKEN_NUMBER, buffer);
            continue;
        }

        if (*ptr == '[') {
            ptr++;
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (*ptr != ']' && *ptr != '\0') {
                buffer[length++] = *ptr++;
            }
            buffer[length] = '\0';
            if (*ptr == ']') {
                ptr++; // Skip the closing bracket
            }
            tokens[tokenIndex++] = *createToken(TOKEN_LIST, buffer);
            continue;
        }

        if (*ptr == '(') {
            ptr++;
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (*ptr != ')' && *ptr != '\0') {
                buffer[length++] = *ptr++;
            }
            buffer[length] = '\0';
            if (*ptr == ')') {
                ptr++; // Skip the closing parenthises
            }
            tokens[tokenIndex++] = *createToken(TOKEN_PARENTHISES, buffer);
            continue;
        }

        if (*ptr == '+' || *ptr == '-' || *ptr == '*' || *ptr == '/' ||
                *ptr == '%' || *ptr == '>' || *ptr == '<' || *ptr == '~' ||
                *ptr == '!') {
            if (*ptr == '~' && *(ptr - 1) == '!') {
                tokens[tokenIndex++] = *createToken(TOKEN_OPERATOR, "!~");
                ptr++;
                continue;
            }
            if (*ptr == '!') {
                ptr++;
                continue;
            }
            char buffer[2] = {*ptr, '\0'};
            tokens[tokenIndex++] = *createToken(TOKEN_OPERATOR, buffer);
            ptr++;
            continue;
        }

        if (*ptr == '"') {
            ptr++;
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (*ptr != '"' && *ptr != '\0') {
                buffer[length++] = *ptr++;
            }
            buffer[length] = '\0';
            if (*ptr == '"') {
                ptr++; // Skip the closing quote
            }
            tokens[tokenIndex++] = *createToken(TOKEN_STRING, buffer);
            continue;
        }

        if (*ptr == '\n' || *ptr == '\t') {
            ptr++;
            continue;
        }

        if (*ptr == '`') {
            ptr++;
            char buffer[MAX_TOKEN_LENGTH] = {0};
            int length = 0;
            while (*ptr != '`' && *ptr != '\0') {
                buffer[length++] = *ptr++;
            }
            buffer[length] = '\0';
            if (*ptr == '`') {
                ptr++; // Skip the closing quote
            }
            tokens[tokenIndex++] = *createToken(TOKEN_STRING, buffer);
            continue;
        }
    }

    tokens[tokenIndex] = *createToken(TOKEN_EOF, "");
    return tokens;
}
