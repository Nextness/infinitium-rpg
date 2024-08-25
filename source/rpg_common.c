#include <gmp.h>
#include <time.h>
#include <stdio.h>
#include "rpg_common.h"

typedef struct {
    mpz_t base;
    mpz_t bonus_base;
    mpz_t max;
    mpz_t bonus_max;
    mpz_t current;
} common_vitality_t;

void init_bar_calc(
    common_vitality_t *result, const char *base, const char *bonus_base,
    const char *max, const char *bonus_max, const char *cur)
{
    mpz_init_set_str(result->base, base, 10);
    mpz_init_set_str(result->bonus_base, bonus_base, 10);
    mpz_init_set_str(result->max, max, 10);
    mpz_init_set_str(result->bonus_max, bonus_max, 10);
    mpz_init_set_str(result->current, cur, 10);
}

void logger(common_log_e log, const char *message) {
    char buffer[128];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%d %m %Y %H:%M", t);
    char * template = "[%s] %s: %s\n";
    switch (log) {
        case DEBUG: {
            sprintf(template, buffer, "DEBUG", message);
        } break;
        case INFO: {
        } break;
        case WARNING: {
        } break;
        case ERROR: {
        } break;
        case CRITICAL: {
        } break;
        default: {
        } break;
    }
}

