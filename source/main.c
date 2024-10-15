#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

#include "player.h"
#include "game_state.h"
#include "utils_common.h"
#include "ui_config.c"
#include "wrapper.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

init_raylib_wrapper(rl);

common_return_t
rpg_game_setup(rpg_game_state_t *gs inout())
{
    (void)gs;
    rl.set_trace_log_level(LOG_NONE);
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_init(rpg_game_state_t *gs inout())
{
    common_return_t error;
    using_rpg_ui_config_t(gs, ui_config);
    using_rpg_player_t(gs, player);

    gs->time_frame = 0.0f;
    ui_config->screen_height = 450;
    ui_config->screen_width = 800;

    error = rpg_player_init(player);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to initialization of the player...");
        return error;
    }

#ifdef DEVELOPER_BUILD
    common_log(DEBUG, "Initial player status:");
    common_log(DEBUG, "Current level: %d", player->current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f", player->current_exp, player->previous_exp_requirement, player->max_exp);
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
    if unlikely(result <= 0.0f) {
        player->exp_percentage = 0.0f;
        return common_set_return(COMMON_ERROR, "Value cannot be less than or equal to '0'\n");
    }
    player->exp_percentage = result;
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_logic_loop(rpg_game_state_t *gs inout())
{
    common_return_t error;
    using_rpg_player_t(gs, player);

    player->current_exp += 10.0f;

    error = exp_progress_tracker(player);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to track exp progress...");
        return error;
    }

#ifdef DEVELOPER_BUILD
    common_log(DEBUG, "Current level: %d", player->current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f", player->current_exp, player->previous_exp_requirement, player->max_exp);
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
    rl.init_window(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    rl.set_target_fps(FRAME_RATE);

    Rectangle touch_area = {100, 100, 200, 50};
    Vector2 touch_position = {0};
    int current_gesture = GESTURE_NONE;
    int last_gesture = GESTURE_NONE;

    while (!rl.window_should_close()) {
        last_gesture = current_gesture;
        current_gesture = GetGestureDetected();
        touch_position = GetTouchPosition(0);

        // In game timer has passed
        gs->time_frame += rl.get_frame_time();
        if likely(gs->time_frame >= 1.0f) {
            rpg_game_logic_loop(gs);
            gs->time_frame = 0.0f;
        }

        // Drawing the game
        rl.begin_drawing();
        rl.clear_background(RAYWHITE);
        Color col = {0};
        if (CheckCollisionPointRec(touch_position, touch_area) && (current_gesture != GESTURE_NONE)) {
            col = (Color){255, 0, 255, 255};
        } else {
            col = (Color){255, 0, 0, 255};
        }
        rl.draw_rectangle(100, 100, 200, 50, col);
        rl.end_drawing();
    }

    rl.close_window();
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

    common_log_state_context(&gs, error, "rpg_game_setup",    rpg_game_setup,    "Starting", "Completed successfully");
    common_log_state_context(&gs, error, "rpg_game_init",     rpg_game_init,     "Starting", "Completed successfully");
    common_log_state_context(&gs, error, "rpg_game_running",  rpg_game_running,  "Running",  "Stoping");
    common_log_state_context(&gs, error, "rpg_game_deinit",   rpg_game_deinit,   "Starting", "Completed successfully");
    common_log_state_context(&gs, error, "rpg_game_shutdown", rpg_game_shutdown, "Starting", "Completed successfully");
}

