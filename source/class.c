#include <gmp.h>
#include "rpg_common.h"

typedef struct {
    char *name;
    rpg_common_vitality_t initial_life;
    rpg_common_vitality_t initial_mana;
    rpg_common_vitality_t initial_energy_shield;

    //  TODO: Make new struct specifically for regeneration...
    rpg_common_vitality_t initial_life_regeneration;
    rpg_common_vitality_t initial_mana_regeneration;
    rpg_common_vitality_t initial_energy_shield_regeneration;
    float initial_soul;
} rpg_class_t;

