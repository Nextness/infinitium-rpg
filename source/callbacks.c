#include "callbacks.h"
#include <stdbool.h>

#include "game_state.h"
#include "player.h"

// Upgrade 1

#define upgrade_cost_1() 1.0f + 100.0f * player->upgrades.first_upgrade

bool
on_change_upgrade_1(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = upgrade_cost_1();
    return player->current_exp >= cost;
}

bool
buy_upgrade_1(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = 1.0f + 100.0f * player->upgrades.first_upgrade;
    if (player->current_exp >= cost) {
        player->current_exp -= cost;
        player->upgrades.first_upgrade++;
        return true;
    }
    return false;
}

// Upgrade 2

#define upgrade_cost_2() 100.0f + 1000.0f * player->upgrades.second_upgrade

bool
on_change_upgrade_2(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = upgrade_cost_2();
    return player->current_exp >= cost;
}

bool
buy_upgrade_2(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    float cost = upgrade_cost_2();
    if (player->current_exp > cost) {
        player->current_exp -= cost;
        player->upgrades.second_upgrade++;
        return true;
    }
    return false;
}

