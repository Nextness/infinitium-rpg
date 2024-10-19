#include <stdbool.h>

#include "game_state.h"

bool
buy_upgrade(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);
    if (player->current_exp > 1.0f) {
        player->current_exp -= 1.0f;
        printf("Can buy upgrade\n");
        return true;
    }
    return false;
}

