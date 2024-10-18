#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>
#include <time.h>

#include "player.h"
#include "game_state.h"
#include "ui_config.h"
#include "utils_common.h"
#include "ui_config.c"
#include "wrapper.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

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
    using_rpg_ui_state_t(gs, ui_state);

    gs->time_frame = 0.0f;
    ui_config->screen_height = 450;
    ui_config->screen_width = 800;

    ui_state->current_gesture = GESTURE_NONE;
    ui_state->mouse_position = rl.get_touch_position(0);

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

typedef bool (*common_callback_f)(rpg_game_state_t *gs inout());

common_return_t
rpg_button(Vector2 mouse_position in(),
           Rectangle button_area in(),
           Color button_color in(),
           int border_width in(),
           Color border_color in(),
           char *text in(),
           rpg_game_state_t *gs inout(),
           common_callback_f can_buy_upgrade in())
{
    Color current_button_color = button_color;
    Color tint = {0};
    if (rl.check_collision_point_rec(mouse_position, button_area)) {
        int current_gesture = rl.get_gesture_detected();
        if ((current_gesture == GESTURE_TAP || current_gesture == GESTURE_HOLD) && can_buy_upgrade(gs)) {
            tint = (Color){164, 164, 164, 255};
        } else {
            tint = (Color){216, 216, 216, 255};
        }
        current_button_color = ColorAlphaBlend(current_button_color, button_color, tint);
    } else {
        current_button_color = button_color;
    }
    rl.draw_rectangle_rec(button_area, current_button_color);
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_running(rpg_game_state_t *gs inout())
{
    using_rpg_player_t(gs, player);
    using_rpg_ui_config_t(gs, ui_config);
    using_rpg_ui_state_t(gs, ui_state);

    rl.init_window(ui_config->screen_width, ui_config->screen_height, "First Window");
    rl.set_target_fps(FRAME_RATE);

    Rectangle touch_area = {100, 100, 200, 50};

    char exp[128];
    char bought_str[128];
    int bought = 0;
    float price = 100.0f;
    while (!rl.window_should_close()) {
        ui_state->mouse_position = rl.get_touch_position(0);
        ui_state->current_gesture = rl.get_gesture_detected();

        // In game timer has passed
        gs->time_frame += rl.get_frame_time();
        if likely(gs->time_frame >= 1.0f) {
            rpg_game_logic_loop(gs);
            gs->time_frame = 0.0f;
        }

        // Drawing the game
        rl.begin_drawing();
        rl.clear_background(RAYWHITE);
        sprintf(exp, "%.2f", player->current_exp);
        sprintf(bought_str, "%d", bought);
        DrawText(exp, 100, 50, 24, DARKGRAY);
        DrawText(bought_str, 200, 50, 24, LIGHTGRAY);
        rpg_button(ui_state->mouse_position, (Rectangle){ 100, 100, 200, 50}, RED, NULL, BLACK, NULL, NULL, NULL);
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

