#ifndef LEXER_H
#define LEXER_H

typedef enum {
	 TOKEN_INT,
	 TOKEN_VOID,
	 TOKEN_CLASS,
	 TOKEN_IDENTIFIER,
	 TOKEN_NUMBER,
	 TOKEN_OPERATOR,
	 TOKEN_PUNCTUATION,
	 TOKEN_EOF,
	 TOKEN_UNKNOWN
} TokenType;

typedef struct {
	 TokenType type;
	 char* value;
} Token;

Token* tokenize(const char* source);

#endif