#ifndef BUFFER_H
#define BUFFER_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_SIZE 4096
#define MAX_BUFFER_SIZE 4096

typedef struct _Line_ {
	char buffer[MAX_LINE_SIZE];
	unsigned long size;
	unsigned long cursor;
} Line;

typedef struct _Buffer_Context_ {
	Line *lines;
	unsigned long size;
	unsigned long cursor;
} Buffer_Context;

void line_ins_cursor(Line *line, const char *text);
void line_del_cursor(Line *line);
void line_del(Line *line);

void buffer_init(Buffer_Context *context);
void buffer_free(Buffer_Context *context);

// Common operations
void buffer_push_line(Buffer_Context *context);
void buffer_ins_cursor(Buffer_Context *context, const char *text);
void buffer_del_cursor(Buffer_Context *context);
void buffer_del(Buffer_Context *context);
unsigned long buffer_get_cursor_row(Buffer_Context *context);

// File support
void buffer_read(Buffer_Context* context, FILE* file);
bool buffer_write(Buffer_Context* context, FILE* file, const char* filename);

#endif // !BUFFER_H
