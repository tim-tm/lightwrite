#include "./buffer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void line_ins_cursor(line *lin, const char *text) {
  size_t in_len = strlen(text);
  const size_t max_len = MAX_LINE_SIZE - lin->size;
  if (in_len > max_len) {
    in_len = max_len;
  }

  memmove(lin->buffer + lin->cursor + in_len, lin->buffer + lin->cursor,
          lin->size - lin->cursor);
  memcpy(lin->buffer + lin->cursor, text, in_len);
  lin->size += in_len;
  lin->cursor += in_len;
}

void line_del_cursor(line *lin) {
  if (lin->cursor > 0 && lin->size > 0) {
    memmove(lin->buffer + lin->cursor - 1, lin->buffer + lin->cursor,
            lin->size - lin->cursor);
    lin->size--;
    lin->buffer[lin->size] = '\0';
    lin->cursor--;
  }
}

void line_del(line *lin) {
  if (lin->cursor < lin->size && lin->size > 0) {
    memmove(lin->buffer + lin->cursor, lin->buffer + lin->cursor + 1,
            lin->size - lin->cursor);
    lin->size--;
  }
}

void buffer_push_line(buffer_context *context) {
  assert(context);
  assert(context->lines);
  context->size++;
  context->cursor_col++;
  context->lines = realloc(context->lines, context->size * sizeof(line));

  // Zero-out the new fresh buffer
  memset(context->lines[context->size - 1].buffer, 0, MAX_BUFFER_SIZE);
  context->lines[context->size - 1].size = 0;
  context->lines[context->size - 1].cursor = 0;
}

void buffer_prepare(buffer_context *context) {
  assert(context);
  context->size++;
  context->lines = calloc(context->size, sizeof(line));
}

void buffer_ins_cursor(buffer_context *context, const char *text) {
  assert(context);
  assert(context->lines);
  line_ins_cursor(&context->lines[context->size - 1], text);
}

void buffer_del_cursor(buffer_context *context) {
  assert(context);
  assert(context->lines);
  line_del_cursor(&context->lines[context->size - 1]);
}

void buffer_del(buffer_context *context) {
  assert(context);
  assert(context->lines);
  line_del(&context->lines[context->size - 1]);
}

int buffer_get_cursor_row(buffer_context *context) {
  assert(context);
  assert(context->lines);
  return context->lines[context->size - 1].cursor;
}

void buffer_free(buffer_context *context) {
  if (context && context->lines) {
    free(context->lines);
  }
}
