#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>
#include <time.h>
#include <math.h>

#include "player.h"
#include "game_state.h"
#include "ui_config.h"
#include "utils_common.h"
#include "ui_config.c"
#include "ui_components.h"
#include "wrapper.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

common_return_t
rpg_game_setup(rpg_game_state_t *gs inout())
{
    (void)gs;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    rl_set_trace_log_level(LOG_NONE);
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_init(rpg_game_state_t *gs inout())
{
    common_return_t error;
    using_rpg_ui_config_t(gs, ui_config);
    using_rpg_player_t(gs, player);
    using_rpg_ui_state_t(gs, ui_state);

    gs->time_frame = 0.0f;
    ui_config->screen_height = 450;
    ui_config->screen_width = 800;

    ui_state->current_gesture = GESTURE_NONE;
    ui_state->mouse_position = rl_get_touch_position(0);

    error = rpg_player_init(player);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to initialization of the player...");
        return error;
    }

#ifdef DEVELOPER_BUILD
    common_log(DEBUG, "Initial player status:");
    common_log(DEBUG, "Current level: %d", player->current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f",
               player->current_exp, player->previous_exp_requirement, player->max_exp);
#endif

    return common_set_return(COMMON_OK, NULL);
}

common_return_t
next_exp_bump(int next_level in(), float *result out())
{
    const int minimum_exp = 100;
    *result += (float)(10 * next_level + minimum_exp);
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
exp_progress_tracker(rpg_player_t *player inout())
{
    float max_exp = player->max_exp - player->previous_exp_requirement;
    float current_exp = player->current_exp - player->previous_exp_requirement;
    float result = 1 - (max_exp - current_exp) / max_exp;
    if (result < 0.0f) result = 0.0f;
    player->exp_percentage = result;
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_logic_loop(rpg_game_state_t *gs inout())
{
    common_return_t error;
    using_rpg_player_t(gs, player);

    player->current_exp += 1.0f;
    if (player->upgrades.first_upgrade > 0) {
        player->current_exp += player->upgrades.first_upgrade * 10.0f;
    }

    if (player->upgrades.second_upgrade > 0) {
        player->current_exp += player->upgrades.second_upgrade * 30.0f;
    }

    error = exp_progress_tracker(player);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to track exp progress...");
        return error;
    }

#ifdef DEVELOPER_BUILD
    common_log(DEBUG, "Current level: %d", player->current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f",
               player->current_exp, player->previous_exp_requirement, player->max_exp);
#endif

    if (player->exp_percentage >= 1.0f) {
        common_clamp_min(player->exp_percentage, 1.0f);
        error = next_exp_bump(player->current_level++, &player->previous_exp_requirement);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed at bumping exp...");
            return error;
        }

        error = next_exp_bump(player->current_level, &player->max_exp);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed at bumping exp...");
            return error;
        }
    }
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_running(rpg_game_state_t *gs inout())
{
    using_rpg_player_t(gs, player);
    using_rpg_ui_config_t(gs, ui_config);
    using_rpg_ui_state_t(gs, ui_state);


    char exp[128];
    char bought_str[128];
    int bought = 0;
    common_return_t error;
    uic_button_config_t btn = {
        (rl_rectangle_t){100, 100, 200, 50}, BLUE, 4, DARKBLUE, "Hello World", BLACK, 16,
        "upgd-1"
    };
    uic_button_config_t btn2 = {
        (rl_rectangle_t){100, 200, 200, 50}, RED, 4, BLACK, "Hello World", BLACK, 16,
        "updg-2"
    };

    rl_set_target_fps(FRAME_RATE);
    rl_init_window(ui_config->screen_width, ui_config->screen_height, 
                   "Infinitium RPG - Incremental Game");
    while (!rl_window_should_close()) {
        ui_state->mouse_position = rl_get_touch_position(0);
        ui_state->current_gesture = rl_get_gesture_detected();

        // In game timer has passed
        gs->time_frame += rl_get_frame_time();
        if likely(gs->time_frame >= 1.0f) {
            rpg_game_logic_loop(gs);
            gs->time_frame = 0.0f;
        }

        // Drawing the game
        rl_begin_drawing();
        rl_clear_background(RAYWHITE);
        sprintf(exp, "Current Exp: %.2f", player->current_exp);
        sprintf(bought_str, "Upagrade 1: %d", player->upgrades.first_upgrade);
        DrawText(exp, 100, 50, 24, DARKGRAY);
        DrawText(bought_str, 500, 50, 24, LIGHTGRAY);

        error = uic_button(ui_state->mouse_position, btn, gs, &buy_upgrade_1);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed to execute de upgrade button");
        }

        error = uic_button(ui_state->mouse_position, btn2, gs, &buy_upgrade_2);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed to execute de upgrade button");
        }

        rl_end_drawing();
    }
    rl_close_window();

    return common_set_return(COMMON_OK, NULL);
}


common_return_t
rpg_game_deinit(rpg_game_state_t *gs inout())
{
    (void)gs;
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_shutdown(rpg_game_state_t *gs inout())
{
    (void)gs;
    return common_set_return(COMMON_OK, NULL);
}

int
main(void)
{
    rpg_game_state_t gs;
    common_return_t error;

    common_log(TRACE, "Setting up the game configuration");
    error = rpg_game_setup(&gs);
    if unlikely(common_get_error(error) != COMMON_OK) {
        const char *error_reason =  common_get_error_msg(error);
        common_log(ERROR, "Failed to setup the game - reason: %s", error_reason);
        return -1;
    }
    common_log(TRACE, "Successfully configured the game");

    common_log(TRACE, "Initializing the game configuration");
    error = rpg_game_init(&gs);
    if unlikely(common_get_error(error) != COMMON_OK) {
        const char *error_reason = common_get_error_msg(error);
        common_log(ERROR, "Failed to initialize the game - reason: %s", error_reason);
        return -1;
    }
    common_log(TRACE, "Successfully initialized the game");

    common_log(TRACE, "Running the game");
    error = rpg_game_running(&gs);
    if unlikely(common_get_error(error) != COMMON_OK) {
        const char *error_reason = common_get_error_msg(error);
        common_log(ERROR, "Failed to run the game - reason: %s", error_reason);
        return -1;
    }
    common_log(TRACE, "Successfully run the game");

    common_log(TRACE, "De-initializing the game");
    error = rpg_game_deinit(&gs);
    if unlikely(common_get_error(error) != COMMON_OK) {
        const char *error_reason = common_get_error_msg(error);
        common_log(ERROR, "Failed to de-initialize the game - reason: %s", error_reason);
        return -1;
    }
    common_log(TRACE, "Successfully de-initialized the game");

    common_log(TRACE, "Shuttingdown the game");
    error = rpg_game_shutdown(&gs);
    if unlikely(common_get_error(error) != COMMON_OK) {
        const char *error_reason = common_get_error_msg(error);
        common_log(ERROR, "Failed to shutdown the game - reason: %s", error_reason);
        return -1;
    }
    common_log(TRACE, "Successfully shutdown the game");

    return 0;
}

