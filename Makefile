CC=gcc
CFLAGS=-Wall -Wextra -pedantic
LIBS=-lSDL2 -lSDL2_ttf -lm

lightwrite: src/main.c
	$(CC) $(CFLAGS) -o bin/lightwrite src/main.c $(LIBS)

clean:
	rm bin/lightwrite
