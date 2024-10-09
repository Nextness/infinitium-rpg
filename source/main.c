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

#define in
#define out
#define inout

typedef struct {
    rpg_ui_config_t ui_config;
//    rpg_player_t player;
} rpg_game_state_t;

#define GET_SH(game_state) (game_state)->ui_config.screen_height
#define GET_SW(game_state) (game_state)->ui_config.screen_width

void rpg_beging_ui_context() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
}

void rpg_end_ui_context(void) {
    EndDrawing();
}

common_return_t rpg_game_setup(in rpg_game_state_t *gs) {
    (void)gs;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_init(inout rpg_game_state_t *gs) {
    gs->ui_config.screen_width = 800;
    gs->ui_config.screen_height = 450;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

common_return_t progress_tracker(in float max_exp, in float current_exp, out float *result) {
    float temp = 1 - ((max_exp - current_exp) / max_exp);
    if (temp >= 0) {
        return COMMON_SET_RETURN(COMMON_ERROR, "Value cannot equal to '0'\n");
    }
    result = &temp;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

float next_exp_bump(int next_level) {
    const int minimum_exp = 100;
    return (float)(10 * next_level + minimum_exp);
}

float exp_progress_tracker(float current_required_exp, float current_exp, float previous_required_exp) {
    current_required_exp -= previous_required_exp;
    current_exp -= previous_required_exp;
    return (1 - (current_required_exp - current_exp) / current_required_exp);
}

common_return_t rpg_game_running(inout rpg_game_state_t *gs) {
    InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    SetTargetFPS(FRAME_RATE);

    // rpg_common_skills_t skills = rpg_common_starter_skills();
    float time_frame = 0.0f;
    float current_exp = 0.0f;
    float max_exp = 100.0f;
    float exp_percentage = 0.0f;
    int current_level = 0;
    float previous_exp_requirement = 0.0f;

    // In game timer
    while (!WindowShouldClose()) {
        if ((time_frame += GetFrameTime()) >= 1.0f) {
            current_exp += 2.0f;
            exp_percentage = exp_progress_tracker(max_exp, current_exp, previous_exp_requirement);
            printf("Current level: %d\n", current_level);
            printf("Current EXP %f | Previous required exp %f | max_exp %f\n", current_exp, previous_exp_requirement, max_exp);
            if (exp_percentage >= 1.0f) {
                if (exp_percentage > 1.0f) exp_percentage = 1.0f;
                previous_exp_requirement += next_exp_bump(current_level);
                current_level++;
                max_exp += next_exp_bump(current_level);
            }
            time_frame = 0.0f;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}


common_return_t rpg_game_deinit(void) {
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_shutdown(inout rpg_game_state_t *gs) {
    (void)gs;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

int main(void) {
    common_return_t ret;
    rpg_game_state_t gs;

    ret = rpg_game_setup(&gs);
    COMMON_CHECK_ASSERT_ERROR(ret, "rpg_game_setup");

    ret = rpg_game_init(&gs);
    COMMON_CHECK_ASSERT_ERROR(ret, "rpg_game_init");

    ret = rpg_game_running(&gs);
    COMMON_CHECK_ASSERT_ERROR(ret, "rpg_game_running");

    ret = rpg_game_deinit();
    COMMON_CHECK_ASSERT_ERROR(ret, "rpg_game_deinit");

    ret = rpg_game_shutdown(&gs);
    COMMON_CHECK_ASSERT_ERROR(ret, "rpg_game_shutdown");
}

