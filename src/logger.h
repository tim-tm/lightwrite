#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdbool.h>

typedef enum _Log_Level_ {
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,

    LOG_LEVEL_MAX_TAG
} Log_Level;

bool logger_init(void);
void logger_destroy(void);

void logger_print(Log_Level level, const char* message, ...);

#ifndef LOG_FATAL
#define LOG_FATAL(...) logger_print(LOG_LEVEL_FATAL, __VA_ARGS__)
#endif // !LOG_FATAL

#ifndef LOG_ERROR
#define LOG_ERROR(...) logger_print(LOG_LEVEL_ERROR, __VA_ARGS__)
#endif // !LOG_ERROR

#ifndef LOG_WARN
#define LOG_WARN(...) logger_print(LOG_LEVEL_WARN, __VA_ARGS__)
#endif // !LOG_WARN

#ifdef _DEBUG
#ifndef LOG_DEBUG
#define LOG_DEBUG(...) logger_print(LOG_LEVEL_DEBUG, __VA_ARGS__)
#endif // !LOG_DEBUG
#else
#define LOG_DEBUG(...)
#endif // _DEBUG

#ifndef LOG_INFO
#define LOG_INFO(...) logger_print(LOG_LEVEL_INFO, __VA_ARGS__)
#endif // !LOG_INFO

#endif // !_LOGGER_H_
