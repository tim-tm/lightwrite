#ifndef BUFFER_H
#define BUFFER_H

#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

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

void buffer_push_line(Buffer_Context *context);
void buffer_prepare(Buffer_Context *context);
void buffer_ins_cursor(Buffer_Context *context, const char *text);
void buffer_del_cursor(Buffer_Context *context);
void buffer_del(Buffer_Context *context);
void buffer_free(Buffer_Context *context);
unsigned long buffer_get_cursor_row(Buffer_Context *context);

#endif // !BUFFER_H
