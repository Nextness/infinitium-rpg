#include <gmp.h>
#include "common.c"

typedef struct {
    char *name;
    common_vitality_t initial_life;
    common_vitality_t initial_mana;
    common_vitality_t initial_energy_shield;

    //  TODO: Make new struct specifically for regeneration...
    common_vitality_t initial_life_regeneration;
    common_vitality_t initial_mana_regeneration;
    common_vitality_t initial_energy_shield_regeneration;
    float initial_soul;
} rpg_class_t;

