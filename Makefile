CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g
INCLUDES=
LIBS=-lSDL2 -lSDL2_ttf

SRCDIR=src
BUILDDIR=build
RELEASEDIR=build/release

ifeq ($(BUILD_TYPE), RELEASE)
CFLAGS=-Wall -Wextra -pedantic
BUILDDIR=$(RELEASEDIR)
endif

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

BINARY=$(BUILDDIR)/lightwrite

.PHONY: all setup clean rebuild destroy

all: $(BINARY)

$(BINARY): $(BUILDDIR)/$(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

setup:
	mkdir -p $(RELEASEDIR)
	cp -rf resource $(BUILDDIR)
	cp -rf resource $(RELEASEDIR)

clean:
	rm -rf $(BINARY)
	rm -rf $(OBJ)

rebuild: .WAIT destroy .WAIT setup all

destroy:
	rm -rf $(BUILDDIR)
