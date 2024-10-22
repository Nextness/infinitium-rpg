#ifndef UTILS_COMMON_H_
#define UTILS_COMMON_H_

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifdef __GNUC__
    #define likely(x)   (__builtin_expect(!!(x), 1))
    #define unlikely(x) (__builtin_expect(!!(x), 0))
#else
    #define likely(x)   ((x))
    #define unlikely(x) ((x))
#endif

#define common_clamp_min(value, min) \
do {                                 \
    if ((value) < (min)) {           \
        value = value;               \
        break;                       \
    }                                \
    value = min;                     \
} while (0)

#define common_clamp_max(value, max) \
do {                                 \
    if ((value) > (max)) {           \
        value = value;               \
        break;                       \
    }                                \
    value = max;                     \
} while (0)

// This is used just to make my life easier when
// handling errors. Instead of having returns, I
// just have pointers to the results as input.
// If you don't like it, then fuck you I guess...
#define in()
#define out()
#define inout()

// Same goes here. I can specify if a function accepts null.
// You know what to do if you don't like it :)
#define nullable()

typedef enum {
    COMMON_OK,
    COMMON_ERROR,
} common_result_e;

typedef struct {
    common_result_e res;
    char *dev_error_msg nullable();
    char *user_error_msg nullable();
} common_return_t;

#define common_set_return(common, dev_error_msg, user_error_msg) \
    ((common_return_t) { (common), (dev_error_msg) nullable(), (user_error_msg) nullable() })
#define common_get_error(common_return_t) (common_return_t).res
#define common_get_error_msg(common_return_t) (common_return_t).dev_error_msg
#define common_get_user_error_msg(common_return_t) (common_return_t).user_error_msg

#define common_check_assert_error(common_return_t, failed_at)           \
do {                                                                    \
    if (common_get_error((common_return_t)) == COMMON_ERROR) {          \
        common_log(ERROR, "Failed during %s - reason '%s'",             \
                   (failed_at), common_get_error_msg(common_return_t)); \
        exit(1);                                                        \
    }                                                                   \
} while (0)

typedef enum {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    NONE,
} common_log_e;

#define GLOBAL_LOG_LEVEL 0
#define ROLLING_FILE_LOG

void
common_log_helper(FILE *stream, common_log_e log_level, const char *file_name,
                  const char * function_name, int line_number, const char *fmt, ...);

#define common_log(log_level, fmt, ...) \
    common_log_helper(stderr, (log_level), __FILE__, __func__, __LINE__, (fmt), ##__VA_ARGS__)

#endif // UTILS_COMMON_H_

