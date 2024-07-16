all: main

CC = clang
override CFLAGS += -g -Wall -Wno-unused-result -Wno-return-stack-address -pthread -lm

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -O0 $(SRCS) -o "$@"

clean:
	rm -f nuxi nuxi-debug