#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

void readFile(const char* filename, char** buffer) {
	 FILE* file = fopen(filename, "r");
	 if (!file) {
		  fprintf(stderr, "Error: Could not open file %s\n", filename);
		  exit(1);
	 }

	 fseek(file, 0, SEEK_END);
	 long length = ftell(file);
	 fseek(file, 0, SEEK_SET);

	 *buffer = (char*)malloc(length + 1);
	 if (!*buffer) {
		  fprintf(stderr, "Error: Could not allocate memory for file content\n");
		  exit(1);
	 }

	 fread(*buffer, 1, length, file);
	 (*buffer)[length] = '\0';

	 fclose(file);
}

int main(int argc, char* argv[]) {
	 if (argc < 2) {
		  fprintf(stderr, "Usage: %s <filename.nux>\n", argv[0]);
		  return 1;
	 }

	 const char* filename = argv[1];
	 const char* extension = strrchr(filename, '.');
	 if (!extension || strcmp(extension, ".nux") != 0) {
		  fprintf(stderr, "Error: File must have a .nux extension\n");
		  return 1;
	 }

	 char* sourceCode;
	 readFile(filename, &sourceCode);

	 Token* tokens = tokenize(sourceCode);

	 Parser parser = {tokens, 0};

	 // Generate assembly file
	 char asmFileName[256];
	 snprintf(asmFileName, sizeof(asmFileName), "%s.s", filename);
	 parse(&parser, asmFileName);

	 // Free resources
	 free(sourceCode);
	 for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
		  free(tokens[i].value);
	 }
	 free(tokens);

	 // Compile assembly to object file
	 char command[512];
	 snprintf(command, sizeof(command), "gcc -c %s -o %s.o", asmFileName, filename);
	 int result = system(command);
	 if (result != 0) {
		  fprintf(stderr, "Error: Assembly compilation failed\n");
		  return 1;
	 }

	 return 0;
}