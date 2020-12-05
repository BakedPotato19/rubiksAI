FILES=main.c rubiks.c
CC=gcc
CFLAGS=-g -Wall -Wextra -pendantic -std=gnu99

run:
	$(CC) -o rubiks $(FILES)
	./rubiks
all:
	$(CC) -o rubiks $(FILES)
clean:
	rm -rf rubiks
