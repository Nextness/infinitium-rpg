#ifndef RPG_PLAYER_H_
#define RPG_PLAYER_H_

#include <gmp.h>

#include "utils_common.h"
#include "rpg_common.h"

typedef struct {
    int first_upgrade;
    int second_upgrade;
} rpg_bought_upgrades_t;

typedef struct {
    // display buffer
    char display_current_exp[1024];
    char display_max_exp[1024];
    char display_exp_percentage[1024];
    char display_previous_exp_requirement[1024];
    char display_current_level[1024];

    // logical/mathematical operations
    mpf_t current_exp_adv;
    mpf_t max_exp_adv;
    mpf_t exp_percetage_adv;
    mpf_t previous_exp_requirement_adv;
    mpf_t current_level_adv;

    // Upgrades
    rpg_bought_upgrades_t upgrades;
} rpg_player_t;

#define using_rpg_player_t(gs, player) rpg_player_t *(player) = &(gs)->player

common_return_t
rpg_player_init(rpg_player_t *player inout());

common_return_t
rpg_player_update_display_info(rpg_player_t *player inout());

#endif // RPG_PLAYER_H_
