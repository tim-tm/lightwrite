#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static const char level_str[LOG_LEVEL_MAX_TAG][32] = {
    "\033[1;31m[FATAL]: ",
    "\033[1;35m[ERROR]: ",
    "\033[1;33m[WARNING]: ",
    "\033[1;32m[DEBUG]: ",
    "\033[1;37m[INFO]: "
};

bool logger_init(void) {
    return true;
}

void logger_destroy(void) { }

void logger_print(Log_Level level, const char* message, ...) {
    int message_len = 4096;
    char msg[message_len];
    memset(msg, 0, sizeof(msg));

    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(msg, message_len, message, arg_ptr);
    va_end(arg_ptr);

    printf("%s%s\033[0m\n", level_str[level], msg);
}
