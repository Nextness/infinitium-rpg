#include <gmp.h>

#include "utils_common.h"
#include "player.h"

common_return_t
rpg_player_init(rpg_player_t *player inout())
{
    player->current_exp = 0.0f;
    player->max_exp = 100.0f;
    player->exp_percentage = 0.0f;
    player->current_level = 0;
    player->previous_exp_requirement = 0.0f;
    player->upgrades.first_upgrade = 0;
    player->upgrades.second_upgrade = 0;
    return common_set_return(COMMON_OK, NULL, NULL);
}

