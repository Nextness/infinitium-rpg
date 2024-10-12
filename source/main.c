#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

// #include "rpg_common.h"
#include "player.h"
#include "utils_common.h"
#include "ui_config.c"
#include "wrapper.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

typedef struct {
    float time_frame;
    rpg_ui_config_t ui_config;
    rpg_player_t player;
} rpg_game_state_t;

#define GET_SH(game_state) (game_state)->ui_config.screen_height
#define GET_SW(game_state) (game_state)->ui_config.screen_width

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void
rpg_beging_ui_context(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
}

void
rpg_end_ui_context(void)
{
    EndDrawing();
}

common_return_t
rpg_game_setup(rpg_game_state_t *gs in())
{
    (void)gs;
    SetTraceLogLevel(LOG_NONE);
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_init(rpg_game_state_t *gs inout())
{
    common_return_t error;
    gs->ui_config.screen_width = 800;
    gs->ui_config.screen_height = 450;

    gs->time_frame = 0.0f;

    error = rpg_player_init(&gs->player);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to initialization of the player...");
        return error;
    }

    common_log(DEBUG, "Initial player status:");
    common_log(DEBUG, "Current level: %d", gs->player.current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f", gs->player.current_exp, gs->player.previous_exp_requirement, gs->player.max_exp);

    return common_set_return(COMMON_OK, NULL);
}

common_return_t
progress_tracker(float max_exp in(), float current_exp in(), float *result out())
{
    *result = 1 - ((max_exp - current_exp) / max_exp);
    if (*result <= 0.0f) {
        *result = 0.0f;
        return common_set_return(COMMON_ERROR, "Value cannot less than or equal to '0'\n");
    }
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
exp_progress_tracker(float current_required_exp in(), float current_exp in(),
                     float previous_required_exp in(), float *result out())
{
    current_required_exp -= previous_required_exp;
    current_exp -= previous_required_exp;
    *result = (1 - (current_required_exp - current_exp) / current_required_exp);
    if (*result <= 0) {
        *result = 0.0f;
        return common_set_return(COMMON_ERROR, "Value cannot less than or equal to '0'\n");
    }
    return common_set_return(COMMON_OK, NULL);
}

common_return_t
rpg_game_logic_loop(rpg_game_state_t *gs inout())
{
    common_return_t error;

    gs->player.current_exp += 10.0f;
    error = exp_progress_tracker(gs->player.max_exp, gs->player.current_exp, gs->player.previous_exp_requirement, &gs->player.exp_percentage);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to track exp progress...");
        return error;
    }

    common_log(DEBUG, "Current level: %d", gs->player.current_level);
    common_log(DEBUG, "Current EXP %f | Previous required exp %f | max_exp %f", gs->player.current_exp, gs->player.previous_exp_requirement, gs->player.max_exp);

    if (gs->player.exp_percentage >= 1.0f) {
        if (gs->player.exp_percentage >= 1.0f) gs->player.exp_percentage = 1.0f;

        // Be careful in here because of the post increment...
        error = next_exp_bump((gs->player.current_level)++, &gs->player.previous_exp_requirement);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed at bumping exp...");
            return error;
        }

        error = next_exp_bump(gs->player.current_level, &gs->player.max_exp);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed at bumping exp...");
            return error;
        }
    }
    return common_set_return(COMMON_OK, NULL);
}

init_raylib_wrapper(rl);

common_return_t
rpg_game_running(rpg_game_state_t *gs inout())
{
    rl.init_window(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    rl.set_target_fps(FRAME_RATE);

    while (!rl.window_should_close()) {
        // In game timer has passed
        if ((gs->time_frame += rl.get_frame_time()) >= 1.0f) {
            rpg_game_logic_loop(gs);
            gs->time_frame = 0.0f;
        }

        // Drawing the game
        rl.begin_drawing();
        rl.clear_background(RAYWHITE);
            rl.draw_rectangle(100, 100, 200, 50, (Color){255, 00, 00, 255});
        rl.end_drawing();
    }

    rl.close_window();
    // CloseWindow();
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

