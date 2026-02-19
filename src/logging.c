#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static LogLevel current_log_level = LOG_LEVEL_INFO;

void log_set_level(LogLevel level)
{
    current_log_level = level;
}

static void log_message(LogLevel level, const char *level_str, const char *fmt, va_list args)
{
    if (level < current_log_level)
    {
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_buffer[26];
    strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(stdout, "[%s] [%s] ", time_buffer, level_str);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
}

void log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_message(LOG_LEVEL_DEBUG, "DEBUG", fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_message(LOG_LEVEL_INFO, "INFO", fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_message(LOG_LEVEL_WARN, "WARN", fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_message(LOG_LEVEL_ERROR, "ERROR", fmt, args);
    va_end(args);
}
