#include "buffer.h"
#include "lassert.h"

#include <stdbool.h>
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
	LASSERT(context);
	context->cursor = 0;
	context->size = 1;
    context->capacity = BUFFER_DEFAULT_CAPACITY;
	context->lines = calloc(context->capacity, sizeof(Line));
}

void buffer_free(Buffer_Context *context) {
	if (context && context->lines) {
		free(context->lines);
	}
}

void buffer_push_line(Buffer_Context *context) {
	LASSERT(context);
	LASSERT(context->lines);
	context->size++;
	context->cursor++;

    if (context->size >= context->capacity) {
	    context->capacity = context->capacity == 0 ? BUFFER_DEFAULT_CAPACITY : context->capacity * 2;
        context->lines = realloc(context->lines, context->capacity * sizeof(Line));
    }

	// Zero-out the new fresh buffer
	memset(context->lines[context->size - 1].buffer, 0, MAX_BUFFER_SIZE);
	context->lines[context->size - 1].size = 0;
	context->lines[context->size - 1].cursor = 0;
}

void buffer_ins_cursor(Buffer_Context *context, const char *text) {
	LASSERT(context);
	LASSERT(context->lines);

    line_ins_cursor(&context->lines[context->cursor], text);
}

void buffer_del_cursor(Buffer_Context *context) {
	LASSERT(context);
	LASSERT(context->lines);
	line_del_cursor(&context->lines[context->cursor]);
}

void buffer_del(Buffer_Context *context) {
	LASSERT(context);
	LASSERT(context->lines);
	line_del(&context->lines[context->cursor]);
}

size_t buffer_get_cursor_row(Buffer_Context *context) {
	LASSERT(context);
	LASSERT(context->lines);
	return context->lines[context->cursor].cursor;
}

void buffer_read(Buffer_Context* context, FILE* file) {
    LASSERT(context);
    LASSERT(file);
    
    buffer_free(context);
    // Clear the buffer.
    buffer_init(context);

    char* line = NULL;
    size_t length = 0;

    // NOTE: getline is only avalible for systems on _POSIX_C_SOURCE >= 200809L
    // FIXME: This segfaults if the file is a binary file, check for that!!
    while (getline(&line, &length, file) != -1) {
        unsigned int linelen = strlen(line);
        if (linelen > MAX_BUFFER_SIZE) {
            printf("Line ignored: %zu > %u\n", length, MAX_LINE_SIZE);
            return;
        }

        line[linelen-1] = '\0';
        buffer_ins_cursor(context, line);
        buffer_push_line(context);
    }
}

bool buffer_write(Buffer_Context* context, FILE* file, const char* filename) {
    LASSERT(context);

    // closing unopened files will seg-fault
    if (file) {
        fclose(file);
    }

    // Open the file for writing (and create it if it doesn't already exist)
    file = fopen(filename, "w+");
    // Check for errors anyways, since it may want to create a file in write-protected area or any other error may occour.
    if (!file) {
        printf("Failed to open/create: %s\n", filename);
        return false;
    }
    
    for (size_t i = 0; i < context->size; ++i) {
        char* line = context->lines[i].buffer;
        
        if (i != context->size-1) {
            line[context->lines[i].size] = '\n';
        } else {
            line[context->lines[i].size] = '\0';
        }
        
        fprintf(file, "%s", line);
    }
    return true;
}
