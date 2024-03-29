MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = lightwrite
lightwrite_SOURCES = \
					buffer.c \
					buffer.h \
					filemanager.c \
					filemanager.h \
					font.c \
					font.h \
					keybinds.c \
					keybinds.h \
					lassert.c \
					lassert.h \
					logger.c \
					logger.h \
					main.c
lightwrite_CFLAGS = -DDATA_DIR=\""$(DESTDIR)$(pkgdatadir)"\"
lightwrite_CPPFLAGS = @SDL2_CFLAGS@
lightwrite_LDFLAGS = @SDL2_LIBS@ -lSDL2_ttf
