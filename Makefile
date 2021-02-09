CC = gcc
CFLAGS = -std=c99
doenv: source.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f doenv
