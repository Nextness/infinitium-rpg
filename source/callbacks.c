#include <stdbool.h>

#include "callbacks.h"
#include "game_state.h"
#include "player.h"

// Upgrade 1

bool
on_change_upgrade_1_adv(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);

    mpf_t cost, tmp;
    mpf_init_set_str(cost, "1.0", 10);
    mpf_init_set_str(tmp, "100.0", 10);
    mpf_mul_ui(tmp, tmp, player->upgrades.first_upgrade);
    mpf_add(cost, cost, tmp);

    return mpf_cmp(player->current_exp_adv, cost) >= 0;
}

bool
buy_upgrade_1_adv(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);

    mpf_t cost, tmp;
    mpf_init_set_str(cost, "1.0", 10);
    mpf_init_set_str(tmp, "100.0", 10);
    mpf_mul_ui(tmp, tmp, player->upgrades.first_upgrade);
    mpf_add(cost, cost, tmp);

    if (mpf_cmp(player->current_exp_adv, cost) >= 0) {
        mpf_sub(player->current_exp_adv, player->current_exp_adv, cost);
        player->upgrades.first_upgrade++;
        return true;
    }

    return false;
}

// Upgrade 2

bool
on_change_upgrade_2_adv(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);

    mpf_t cost, tmp;
    mpf_init_set_str(cost, "100.0", 10);
    mpf_init_set_str(tmp, "1000.0", 10);
    mpf_mul_ui(tmp, tmp, player->upgrades.second_upgrade);
    mpf_add(cost, cost, tmp);

    return mpf_cmp(player->current_exp_adv, cost) >= 0;
}

bool
buy_upgrade_2_adv(rpg_game_state_t *gs)
{
    using_rpg_player_t(gs, player);

    mpf_t cost, tmp;
    mpf_init_set_str(cost, "100.0", 10);
    mpf_init_set_str(tmp, "1000.0", 10);
    mpf_mul_ui(tmp, tmp, player->upgrades.second_upgrade);
    mpf_add(cost, cost, tmp);

    if (mpf_cmp(player->current_exp_adv, cost) >= 0) {
        mpf_sub(player->current_exp_adv, player->current_exp_adv, cost);
        player->upgrades.second_upgrade++;
        return true;
    }

    return false;
}

