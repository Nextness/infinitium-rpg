#ifndef UTILS_COMMON_H_
#define UTILS_COMMON_H_

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    COMMON_OK,
    COMMON_ERROR,
} common_result_e;

typedef struct {
    common_result_e res;
    char *error_msg;
} common_return_t;

#define common_set_return(common, error) ((common_return_t) { (common), (error) })
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
common_log(common_log_e log_level, const char *message, ...);

#endif // UTILS_COMMON_H_

