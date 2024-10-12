#ifndef UTILS_COMMON_H_
#define UTILS_COMMON_H_

#include <stdio.h>
#include <stdarg.h>

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
    char *error_msg nullable();
} common_return_t;

#define common_set_return(common, error) ((common_return_t) { (common), (error) nullable() })
#define common_get_error(common_return_t) (common_return_t).res
#define common_get_error_msg(common_return_t) (common_return_t).error_msg

#define common_check_assert_error(common_return_t, failed_at)           \
do {                                                                    \
    if (common_get_error((common_return_t)) == COMMON_ERROR) {          \
        common_log(ERROR, "Failed during %s - reason '%s'",             \
                   (failed_at), common_get_error_msg(common_return_t)); \
        exit(1);                                                        \
    }                                                                   \
} while (0)

#define common_log_state_context(game_state, error, function_name, callable, start_msg, completed_msg) \
while (true) {                                                                                         \
    common_log(TRACE, "%s - State: %s", (function_name), (start_msg));                                 \
    (error) = (callable)((game_state));                                                                \
    common_check_assert_error((error), (function_name));                                               \
    common_log(TRACE, "%s - State: %s", (function_name), (completed_msg));                             \
    break; \
}

typedef enum {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    NONE,
} common_log_e;

void
common_log(common_log_e log_level, const char *fmt, ...);

#endif // UTILS_COMMON_H_

