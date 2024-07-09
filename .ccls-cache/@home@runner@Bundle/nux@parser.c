#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token currentToken(Parser *parser) { return parser->tokens[parser->current]; }

void advance(Parser *parser) { parser->current++; }

void generateFunctionAssembly(FILE *file, Token funcName) {
  fprintf(file, ".global %s\n", funcName.value);
  fprintf(file, "%s:\n", funcName.value);
  fprintf(file, "    ret\n");
}

void generateClassAssembly(FILE *file, Token className) {
  // Classes can have multiple functions, we handle those within this scope.
  fprintf(file, "; Class %s\n", className.value);
}

void parseFunction(Parser *parser, FILE *file) {
  advance(parser); // Skip 'void' or 'int'
  Token funcName = currentToken(parser);
  advance(parser); // Skip function name
  advance(parser); // Skip '('
  advance(parser); // Skip ')'
  advance(parser); // Skip '{'

  printf("Parsing function: %s\n", funcName.value);
  generateFunctionAssembly(file, funcName);

  while (strcmp(currentToken(parser).value, "}") != 0) {
    advance(parser);
  }
  advance(parser); // Skip '}'
}

void parseClass(Parser *parser, FILE *file) {
  advance(parser); // Skip 'class'
  Token className = currentToken(parser);
  advance(parser); // Skip class name
  advance(parser); // Skip '{'

  printf("Parsing class: %s\n", className.value);
  generateClassAssembly(file, className);

  while (strcmp(currentToken(parser).value, "}") != 0) {
    if (currentToken(parser).type == TOKEN_VOID ||
        currentToken(parser).type == TOKEN_INT) {
      parseFunction(parser, file);
    } else {
      advance(parser);
    }
  }
  advance(parser); // Skip '}'
}

void parse(Parser *parser, const char *outputFileName) {
  FILE *file = fopen(outputFileName, "w");
  if (!file) {
    fprintf(stderr, "Error: Could not open output file %s\n", outputFileName);
    exit(1);
  }

  while (currentToken(parser).type != TOKEN_EOF) {
    if (currentToken(parser).type == TOKEN_VOID ||
        currentToken(parser).type == TOKEN_INT) {
      parseFunction(parser, file);
    } else if (currentToken(parser).type == TOKEN_CLASS) {
      parseClass(parser, file);
    } else {
      advance(parser);
    }
  }

  fclose(file);
}