#ifndef RPG_COMMON_H_
#define RPG_COMMON_H_

#include <raylib.h>

typedef enum {
    COMMON_OK,
    COMMON_ERROR,
} common_result_e;

typedef struct {
    common_result_e res;
    char *error_msg;
} common_return_t;

#define COMMON_SET_RETURN(common, error) ((common_return_t) { (common), (error) })

#define COMMON_GET_RETURN(common_return_t) (common_return_t).res

#define COMMON_CHECK_ASSERT_ERROR(common_return_t, failed_at)                               \
do {                                                                                        \
    if (COMMON_GET_RETURN((common_return_t)) == COMMON_ERROR) {                             \
        printf("Failed during %s - reason '%s'", (failed_at), (common_return_t).error_msg); \
        exit(1);                                                                            \
    }                                                                                       \
} while (0)

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
} common_log_e;

void logger(common_log_e log, const char *message);

#endif // RPG_COMMON_H_
