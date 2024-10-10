#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

#include "rpg_common.h"
#include "utils_common.h"
#include "ui_config.c"
// #include "player.h"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

typedef struct {
    rpg_ui_config_t ui_config;
    // rpg_player_t player;
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
    gs->ui_config.screen_width = 800;
    gs->ui_config.screen_height = 450;
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
rpg_game_logic_loop(float *current_exp inout(), float *max_exp inout(),
                    float *previous_exp_requirement inout(), float *exp_percentage inout(),
                    int *current_level inout())
{
    common_return_t error;
    *current_exp += 10.0f;
    error = exp_progress_tracker(*max_exp, *current_exp, *previous_exp_requirement, exp_percentage);
    if unlikely(common_get_error(error) != COMMON_OK) {
        common_log(ERROR, "Failed to track exp progress...");
        return error;
    }

    common_log(INFO, "Current level: %d", *current_level);
    common_log(INFO, "Current EXP %f | Previous required exp %f | max_exp %f", *current_exp, *previous_exp_requirement, *max_exp);

    if (*exp_percentage >= 1.0f) {
        if (*exp_percentage >= 1.0f) *exp_percentage = 1.0f;

        // Be careful in here because of the post increment...
        error = next_exp_bump((*current_level)++, previous_exp_requirement);
        if unlikely(common_get_error(error) != COMMON_OK) {
            common_log(ERROR, "Failed at bumping exp...");
            return error;
        }

        error = next_exp_bump(*current_level, max_exp);
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
    InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    SetTargetFPS(FRAME_RATE);

    // rpg_common_skills_t skills = rpg_common_starter_skills();
    float time_frame = 0.0f;
    float current_exp = 0.0f;
    float max_exp = 100.0f;
    float exp_percentage = 0.0f;
    int current_level = 0;
    float previous_exp_requirement = 0.0f;
    // common_return_t error;

    while (!WindowShouldClose()) {
        // In game timer
        if ((time_frame += GetFrameTime()) >= 1.0f) {
            rpg_game_logic_loop(&current_exp, &max_exp, &previous_exp_requirement, &exp_percentage, &current_level);
            time_frame = 0.0f;
        }

        // Drawing the game
        BeginDrawing();
        ClearBackground(RAYWHITE);
            DrawRectangle(100, 100, 200, 50, (Color){255, 00, 00, 255});
        EndDrawing();
    }

    CloseWindow();
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

