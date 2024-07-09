/*
 * Please read the main.c file for license.
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
  Token *tokens;
  int current;
} Parser;

void parse(Parser *parser, const char *outputFileName);
void advance(Parser *parser);
void retreat(Parser *parser);

#endif