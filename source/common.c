#include <gmp.h>

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



