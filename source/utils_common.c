#include <stdio.h>
#include <stdarg.h>
#include "utils_common.h"

#define GLOBAL_LOG_LEVEL 0

void
__log_helper(FILE *stream, const char *tag, const char *fmt, va_list args)
{
    fprintf(stream, "[%s] ", tag);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void
common_log(common_log_e log_level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    switch (log_level) {
        case TRACE:    if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stdout, "TRACE", fmt, args);    break;
        case DEBUG:    if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stdout, "DEBUG", fmt, args);    break;
        case INFO:     if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stdout, "INFO", fmt, args);     break;
        case WARNING:  if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stdout, "WARNING", fmt, args);  break;
        case ERROR:    if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stderr, "ERROR", fmt, args);    break;
        case CRITICAL: if (log_level >= GLOBAL_LOG_LEVEL) __log_helper(stderr, "CRITICAL", fmt, args); break;
        case NONE: /* Do nothing when is NONE */; break;
    }
    va_end(args);
}

