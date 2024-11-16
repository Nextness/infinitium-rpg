#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>
#include <time.h>
#include <assert.h>

#include "player.h"
#include "game_state.h"
#include "ui_config.h"
#include "ui_state.h"
#include "utils_common.h"
#include "ui_config.c"
#include "ui_components.h"
#include "wrapper.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100
#define ERROR_FMT "Failed to %s - reason: %s"

common_return_t
rpg_game_setup(rpg_game_state_t *gs inout())
{
    (void)gs;
    rl_set_config_flags(FLAG_MSAA_4X_HINT);
    rl_set_trace_log_level(LOG_NONE);
    rl_set_target_fps(FRAME_RATE);
    return common_set_return(COMMON_OK, NULL, NULL);
}

common_return_t
rpg_game_init(rpg_game_state_t *gs inout())
{
    common_set_defer();

    gs->time_frame = 0.0f;

    using_rpg_ui_config_t(gs, ui_config);
    unlikely_error(ui_init_set_config(ui_config)) {
        common_log(ERROR, ERROR_FMT, "initialize the UI config", common_get_error_msg(error));
        common_defer;
    }

    using_rpg_ui_state_t(gs, ui_state);
    unlikely_error(ui_state_init_config(ui_state)) {
        common_log(ERROR, ERROR_FMT, "initialize the UI state", common_get_error_msg(error));
        common_defer;
    }

    using_rpg_player_t(gs, player);
    unlikely_error(rpg_player_init(player)) {
        common_log(ERROR, ERROR_FMT, "initialize the player", common_get_error_msg(error));
        common_defer;
    }

    unlikely_error(rpg_player_update_display_info(player)) {
        common_log(ERROR, ERROR_FMT, "update display information during initialization",
                   common_get_error_msg(error));
        common_defer;
    }

    common_return;
}

common_return_t
rpg_game_logic_step(rpg_game_state_t *gs inout())
{
#ifdef DEVELOPER_BUILD
    assert(gs != NULL && "game state cannot be null");
#endif

    common_set_defer();

    using_rpg_player_t(gs, player);

    mpf_t gain;
    mpf_init(gain);
    mpf_set_str(gain, "1.0", 10);

    mpf_add(player->current_exp_adv, player->current_exp_adv, gain);
    if (player->upgrades.first_upgrade > 0) {
        mpf_set_str(gain, "10.0", 10);
        mpf_add(player->current_exp_adv, player->current_exp_adv, gain);
    }

    if (player->upgrades.second_upgrade > 0) {
        mpf_set_str(gain, "30.0", 10);
        mpf_add(player->current_exp_adv, player->current_exp_adv, gain);
    }

    mpf_clear(gain);

    common_return;
}

common_return_t
rpg_game_running(rpg_game_state_t *gs inout())
{

#ifdef DEVELOPER_BUILD
    assert(gs != NULL && "game state cannot be null");
#endif

    common_set_defer();

    using_rpg_player_t(gs, player);
    using_rpg_ui_config_t(gs, ui_config);
    using_rpg_ui_state_t(gs, ui_state);

    rl_init_window(ui_config->screen_width, ui_config->screen_height,
                   "Infinitium RPG - Incremental Game");
    while (!rl_window_should_close()) {
        ui_state->mouse_position = rl_get_touch_position(0);
        ui_state->current_gesture = rl_get_gesture_detected();

        gs->time_frame += rl_get_frame_time();
        if likely(gs->time_frame >= 1.0f) {
            rpg_game_logic_step(gs);
            rpg_player_update_display_info(player);
            common_log(DEBUG, "Current player exp: %s", player->display_current_exp);
            gs->time_frame = 0.0f;
        }

        rl_begin_drawing();
        rl_clear_background(RAYWHITE);
        DrawText(player->display_current_exp, 100, 50, 24, DARKGRAY);

        unlikely_error(uic_button(gs, 0)) {
             common_log(ERROR, ERROR_FMT,
                        "execute/render the upgrade button", common_get_error_msg(error));
        }

        unlikely_error(uic_button(gs, 1)) {
             common_log(ERROR, ERROR_FMT,
                        "execute/render the upgrade button", common_get_error_msg(error));
        }

        // TODO: Evaluate if this function call should be executed every game tick
        rpg_player_update_display_info(player);
        rl_end_drawing();
    }
    rl_close_window();

    return common_set_return(COMMON_OK, NULL, NULL);
}


common_return_t
rpg_game_deinit(rpg_game_state_t *gs inout())
{

#ifdef DEVELOPER_BUILD
    assert(gs != NULL && "game state cannot be null");
#endif

    (void)gs;
    return common_set_return(COMMON_OK, NULL, NULL);
}

common_return_t
rpg_game_shutdown(rpg_game_state_t *gs inout())
{

#ifdef DEVELOPER_BUILD
    assert(gs != NULL && "game state cannot be null");
#endif

    (void)gs;
    return common_set_return(COMMON_OK, NULL, NULL);
}

common_return_t
main_game(void)
{
    common_set_defer();
    rpg_game_state_t gs = {0};

    common_log(TRACE, "Setting up the game configuration");
    unlikely_error(rpg_game_setup(&gs)) {
        common_log(ERROR, ERROR_FMT, "setup the game", common_get_error_msg(error));
        common_defer;
    }
    common_log(TRACE, "Successfully configured the game");

    common_log(TRACE, "Initializing the game configuration");
    unlikely_error(rpg_game_init(&gs)) {
        common_log(ERROR, ERROR_FMT, "initialize the game", common_get_error_msg(error));
        common_defer;
    }
    common_log(TRACE, "Successfully initialized the game");

    common_log(TRACE, "Running the game");
    unlikely_error(rpg_game_running(&gs)) {
        common_log(ERROR, ERROR_FMT, "run the game", common_get_error_msg(error));
        common_defer;
    }
    common_log(TRACE, "Successfully run the game");

    common_log(TRACE, "De-initializing the game");
    unlikely_error(rpg_game_deinit(&gs)) {
        common_log(ERROR, ERROR_FMT, "de-initialize the game", common_get_error_msg(error));
        common_defer;
    }
    common_log(TRACE, "Successfully de-initialized the game");

    common_log(TRACE, "Shuttingdown the game");
    unlikely_error(rpg_game_shutdown(&gs)) {
        common_log(ERROR, ERROR_FMT, "shutdown the game", common_get_error_msg(error));
        common_defer;
    }
    common_log(TRACE, "Successfully shutdown the game");

    common_return;
}

int
main(void)
{
    common_return_t error = main_game();
    return common_get_error(error) == COMMON_OK;
}

