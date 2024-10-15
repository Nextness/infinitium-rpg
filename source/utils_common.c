#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "utils_common.h"



#define GLOBAL_LOG_LEVEL 0

void
__log_helper(FILE *stream, const char *tag, const char *fmt, va_list args)
{
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    fprintf(stream, "%s - ", tag);
    fprintf(stream, "%d-%d-%d %d:%d:%d - ", 
            time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour,
            time_info->tm_min, time_info->tm_sec);
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

