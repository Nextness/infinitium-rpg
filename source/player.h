#ifndef RPG_PLAYER_H_
#define RPG_PLAYER_H_

#include "utils_common.h"
#include "rpg_common.h"

typedef struct {
    int first_upgrade;
    int second_upgrade;
} rpg_bought_upgrades_t;

typedef struct {
    float current_exp;
    float max_exp;
    float exp_percentage;
    float previous_exp_requirement;
    int current_level;
    rpg_bought_upgrades_t upgrades;
} rpg_player_t;

#define using_rpg_player_t(gs, player) rpg_player_t *(player) = &(gs)->player

common_return_t
rpg_player_init(rpg_player_t *player inout());

#endif // RPG_PLAYER_H_
