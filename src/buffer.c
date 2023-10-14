#include "buffer.h"

// TODO: Implement an own simple logger and assertion tool, that also can print to files.
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void line_ins_cursor(Line *line, const char *text) {
	size_t in_len = strlen(text);
	const size_t max_len = MAX_LINE_SIZE - line->size;
	if (in_len > max_len) {
		in_len = max_len;
	}

	memmove(line->buffer + line->cursor + in_len, line->buffer + line->cursor,
	    line->size - line->cursor);
	memcpy(line->buffer + line->cursor, text, in_len);
	line->size += in_len;
	line->cursor += in_len;
}

void line_del_cursor(Line *line) {
	if (line->cursor > 0 && line->size > 0) {
		memmove(line->buffer + line->cursor - 1, line->buffer + line->cursor,
		    line->size - line->cursor);
		line->size--;
		line->buffer[line->size] = '\0';
		line->cursor--;
	}
}

void line_del(Line *line) {
	if (line->cursor < line->size && line->size > 0) {
		memmove(line->buffer + line->cursor, line->buffer + line->cursor + 1,
		    line->size - line->cursor);
		line->size--;
	}
}

void buffer_init(Buffer_Context *context) {
	assert(context);
	context->cursor = 0;
	context->size = 1;
	context->lines = calloc(context->size, sizeof(Line));
}

void buffer_free(Buffer_Context *context) {
	if (context && context->lines) {
		free(context->lines);
	}
}

void buffer_push_line(Buffer_Context *context) {
	assert(context);
	assert(context->lines);
	context->size++;
	context->cursor++;
	context->lines = realloc(context->lines, context->size * sizeof(Line));

	// Zero-out the new fresh buffer
	memset(context->lines[context->size - 1].buffer, 0, MAX_BUFFER_SIZE);
	context->lines[context->size - 1].size = 0;
	context->lines[context->size - 1].cursor = 0;
}

void buffer_ins_cursor(Buffer_Context *context, const char *text) {
	assert(context);
	assert(context->lines);
	line_ins_cursor(&context->lines[context->cursor], text);
}

void buffer_del_cursor(Buffer_Context *context) {
	assert(context);
	assert(context->lines);
	line_del_cursor(&context->lines[context->cursor]);
}

void buffer_del(Buffer_Context *context) {
	assert(context);
	assert(context->lines);
	line_del(&context->lines[context->cursor]);
}

unsigned long buffer_get_cursor_row(Buffer_Context *context) {
	assert(context);
	assert(context->lines);
	return context->lines[context->cursor].cursor;
}
