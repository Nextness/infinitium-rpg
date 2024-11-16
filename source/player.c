#include <gmp.h>

#include "utils_common.h"
#include "player.h"

common_return_t
rpg_player_init(rpg_player_t *player inout())
{
    mpf_init_set_str(player->current_exp_adv, "0.0", 10);
    mpf_init_set_str(player->max_exp_adv, "100.0", 10);
    mpf_init_set_str(player->exp_percetage_adv, "0.0", 10);
    mpf_init_set_str(player->current_level_adv, "0.0", 10);
    mpf_init_set_str(player->previous_exp_requirement_adv, "0.0", 10);

    player->upgrades.first_upgrade = 0;
    player->upgrades.second_upgrade = 0;
    return common_set_return(COMMON_OK, NULL, NULL);
}

common_return_t
rpg_player_update_display_info(rpg_player_t *player inout())
{
    common_set_defer();
    const int max_length = 1024;
    const int decimal_points = 4;
    int char_count;

    char_count = gmp_snprintf(player->display_current_exp, max_length, "%.*Ff", decimal_points, player->current_exp_adv);
    if unlikely(char_count >= max_length) {
        common_log(ERROR, "Expected %d but got %d when updating display information for 'player->current_exp'", max_length, char_count);
        common_defer;
    }

    char_count = gmp_snprintf(player->display_max_exp, max_length, "%.*Ff", decimal_points, player->max_exp_adv);
    if unlikely(char_count >= max_length) {
        common_log(ERROR, "Expected %d but got %d when updating display information for 'player->max_exp'", max_length, char_count);
        common_defer;
    }

    char_count = gmp_snprintf(player->display_exp_percentage, max_length, "%.*Ff", decimal_points, player->exp_percetage_adv);
    if unlikely(char_count >= max_length) {
        common_log(ERROR, "Expected %d but got %d when updating display information for 'player->exp_percentage'", max_length, char_count);
        common_defer;
    }

    char_count = gmp_snprintf(player->display_current_level, max_length, "%.*Ff", decimal_points, player->current_level_adv);
    if unlikely(char_count >= max_length) {
        common_log(ERROR, "Expected %d but got %d when updating display information for 'player->current_level'", max_length, char_count);
        common_defer;
    }

    char_count = gmp_snprintf(player->display_previous_exp_requirement, max_length, "%.*Ff", decimal_points, player->previous_exp_requirement_adv);
    if unlikely(char_count >= max_length) {
        common_log(ERROR, "Expected %d but got %d when updating display information for 'player->previous_exp_requirement'", max_length, char_count);
        common_defer;
    }

    common_return;
}
