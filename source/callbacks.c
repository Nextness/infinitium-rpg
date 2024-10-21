#include <stdbool.h>

#include "game_state.h"

bool
buy_upgrade_1(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = 1.0f + 100.0f * player->upgrades.first_upgrade;
    if (player->current_exp > cost) {
        player->current_exp -= cost;
        player->upgrades.first_upgrade++;
        return true;
    }
    return false;
}

bool
buy_upgrade_2(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = 100.0f + 1000.0f * player->upgrades.second_upgrade;
    if (player->current_exp > cost) {
        player->current_exp -= cost;
        player->upgrades.second_upgrade++;
        return true;
    }
    return false;
}

