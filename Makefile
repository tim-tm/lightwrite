CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g
LIBS=-lSDL2 -lSDL2_ttf -lm

lightwrite: src/main.c
	$(CC) $(CFLAGS) -o bin/lightwrite src/main.c src/buffer.c src/font.c $(LIBS)

clean:
	rm bin/lightwrite
