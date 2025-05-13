CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
CC=cc

heap: clean main.c
	$(CC) $(CFLAGS) -o heap main.c
	./heap

clean:
	rm -f heap
