#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>

#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

typedef struct _Line_ {
	char buffer[MAX_LINE_SIZE];
	size_t size;
	size_t cursor;
} Line;

typedef struct _Buffer_Context_ {
	Line *lines;
	size_t size;
	size_t cursor;
} Buffer_Context;

void line_ins_cursor(Line *line, const char *text);
void line_del_cursor(Line *line);
void line_del(Line *line);

void buffer_push_line(Buffer_Context *context);
void buffer_prepare(Buffer_Context *context);
void buffer_ins_cursor(Buffer_Context *context, const char *text);
void buffer_del_cursor(Buffer_Context *context);
void buffer_del(Buffer_Context *context);
void buffer_free(Buffer_Context *context);
size_t buffer_get_cursor_row(Buffer_Context *context);

#endif // !BUFFER_H
