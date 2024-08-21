#include <stdint.h>

typedef struct {
    char *name;
    uint64_t life_gain_per_level;
    uint64_t mana_gain_per_level;
    uint64_t energy_shield_gain_per_level;
    float damage_increase_per_level;
    float soul_gain;
} rpg_promotion_t;

