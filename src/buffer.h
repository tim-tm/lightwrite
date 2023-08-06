#ifndef BUFFER_H
#include <stdlib.h>

#define MAX_LINE_SIZE 1024
#define MAX_BUFFER_SIZE 1024

typedef struct s_line {
  char buffer[MAX_LINE_SIZE];
  size_t size;
  size_t cursor;
} line;

typedef struct s_buffer_context {
  line *lines;
  size_t size;
  size_t cursor_col;
} buffer_context;

void line_ins_cursor(line *lin, const char *text);
void line_del_cursor(line *lin);
void line_del(line *lin);

void buffer_push_line(buffer_context *context);
void buffer_prepare(buffer_context *context);
void buffer_ins_cursor(buffer_context *context, const char *text);
void buffer_del_cursor(buffer_context *context);
void buffer_del(buffer_context *context);
void buffer_free(buffer_context *context);
int buffer_get_cursor_row(buffer_context *context);

#endif // !BUFFER_H
