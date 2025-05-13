CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
CC=gcc

all: clean build run

build: main.c 
	$(CC) $(CFLAGS) -o heap main.c

clean:
	rm -f heap
	rm -f main.s

run:
	./heap

asm: compile assemble-and-run

compile:
	$(CC) $(CFLAGS) -S -o heap.s heap.c

assemble-and-run:
	$(CC) -o heap heap.s
	./heap
