CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
CC=cc

all: clean build run

build: main.c 
	$(CC) $(CFLAGS) -o heap main.c

clean:
	rm -f heap

run:
	./heap
