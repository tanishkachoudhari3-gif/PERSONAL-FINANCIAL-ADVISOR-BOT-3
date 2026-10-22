CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11

build:
	mkdir -p bin
	$(CC) $(CFLAGS) -Isrc src/server.c src/engine.c -o bin/finadvisor -lm

run: build
	./bin/finadvisor

clean:
	rm -rf bin
