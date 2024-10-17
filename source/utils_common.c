#include <stdio.h>
#include <stdarg.h>
#include "utils_common.h"

void
common_log_helper(FILE *stream, common_log_e log_level, const char *file_name, const char * function_name, int line_number, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const char *log_level_mapping[7] = {"TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL", "NONE"};
    const char *tag = log_level_mapping[log_level];

#ifdef ROLLING_FILE_LOG
    va_list tmp;
    va_copy(tmp, args);
#endif

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

#ifdef ROLLING_FILE_LOG
    FILE *log_file_stream = 0;
    const char *prefix = "logstream_";
    const char *suffix = ".jsonl";

    char log_file_name[64];
    if (!log_file_stream) {
        sprintf(log_file_name, "%s%s%s", prefix, "inifinitium", suffix);
        log_file_stream = fopen(log_file_name, "a+");
    }

    fprintf(log_file_stream, "{");
    fprintf(log_file_stream, "\"log_level\": \"%s\", ", tag);
    fprintf(log_file_stream, "\"log_datetime\": \"%d-%d-%d %d:%d:%d\", ", 
            time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour,
            time_info->tm_min, time_info->tm_sec);
    fprintf(log_file_stream, "\"file_name\": \"%s\", ", file_name);
    fprintf(log_file_stream, "\"function_name\": \"%s\", ", function_name);
    fprintf(log_file_stream, "\"line_number\": %d, ", line_number);
    fprintf(log_file_stream, "\"log_message\": \"");
    vfprintf(log_file_stream, fmt, tmp);
    fprintf(log_file_stream, "\"}\n");
#endif
    va_end(args);
    va_end(tmp);
}
