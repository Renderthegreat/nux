/*
 * Nux Software License.
 *
 * Copyright (C) 2024 RenderLabs Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name "Nux" or the names of its contributors may not be used to endorse
 *    or promote products derived from this Software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(const char *filename, char **buffer) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Could not open file %s\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  *buffer = (char *)malloc(length + 1);
  if (!*buffer) {
    fprintf(stderr, "Error: Could not allocate memory for file content\n");
    exit(1);
  }

  fread(*buffer, 1, length, file);
  (*buffer)[length] = '\0';

  fclose(file);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename.nux>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  const char *extension = strrchr(filename, '.');
  if (!extension || strcmp(extension, ".nux") != 0) {
    fprintf(stderr, "Error: File must have a .nux extension\n");
    return 1;
  }

  char *sourceCode;
  readFile(filename, &sourceCode);

  Token *tokens = tokenize(sourceCode);

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
  snprintf(command, sizeof(command), "gcc -c %s -o %s.o", asmFileName,
           filename);
  int result = system(command);
  if (result != 0) {
    fprintf(stderr, "Error: Assembly compilation failed\n");
    return 1;
  }

  return 0;
}