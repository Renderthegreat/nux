#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
  Token *tokens;
  int current;
} Parser;

void parse(Parser *parser);

#endif