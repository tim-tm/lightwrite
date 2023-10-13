CC=gcc
CFLAGS=-Wall -Wextra -pedantic
LIBS=-lSDL2 -lSDL2_ttf

# TODO: Create SRC and OBJ variables that can be iterated for faster build with multiple threads.

lightwrite: src/main.c
	$(CC) $(CFLAGS) -o bin/lightwrite src/main.c src/buffer.c src/font.c $(LIBS)

debug: src/main.c
	$(CC) $(CFLAGS) -g -o bin/lightwrite src/main.c src/buffer.c src/font.c $(LIBS)

clean:
	rm bin/lightwrite
