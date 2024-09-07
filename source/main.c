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
//    rpg_player_t player;
} rpg_game_state_t;

#define GET_SH(game_state) (game_state)->ui_config.screen_height
#define GET_SW(game_state) (game_state)->ui_config.screen_width

void rpg_beging_ui_context(Camera3D camera3d) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera3d);
    // BeginMode2D(camera);
}

void rpg_end_ui_context(void) {
    EndMode3D();
    EndDrawing();
}

common_return_t rpg_game_setup(rpg_game_state_t *gs) {
    (void)gs;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_init(rpg_game_state_t *gs) {
    gs->ui_config.screen_width = 800;
    gs->ui_config.screen_height = 450;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

#define MIN(a,b) ((a) < (b) ? (a) : (b))

float progress_tracker(float max_exp, float current_exp) {
    return (1 - ((max_exp - current_exp) / max_exp));
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

common_return_t rpg_game_running(rpg_game_state_t *gs) {

    int tile_width = 100;
    int tile_height = 50;
    int tile_depth = 5;

    InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    SetTargetFPS(FRAME_RATE);
    rpg_common_skills_t skills = rpg_common_starter_skills();

    float time_frame = 0.0f;
    float game_frame = 0.0f;
    Vector2 touch_position = { 0, 0 };
    Vector3 cube_position = { 0.0f, 0.0f, 0.0f };
    Vector3 enemy_cube_position = { 2.0f, 0.0f, 3.0f };
    Camera3D camera3d = {
        { 10.0f, 10.0f, 10.0f },
        cube_position,
        { 0.0f, 1.0f, 0.0f },
        45.0f,
        CAMERA_ORTHOGRAPHIC
    };


    float pos_x = 200;
    float pos_y = 50;
    float playable_width = GET_SW(gs) - 400.0f;
    float playable_height = GET_SH(gs) - 80.0f;
    Rectangle touch_area = { pos_x, pos_y, playable_width, playable_height};

    int current_gesture = GESTURE_NONE;
    int last_gesture = GESTURE_NONE;

    // char *sword_value = "10";
    // char *axe_value = "10";
    // int sword_progress = 0;
    // char *sword_experience = "100";

    int max_length = 6;
    float padding = 0.1f;
    float isometric_factor = 1.0f;

    float current_exp = 0.0f;
    float max_exp = 100.0f;
    float exp_percentage = 0.0f;
    int current_level = 0;

    float previous_exp_requirement = 0.0f;
    while (!WindowShouldClose()) {
        last_gesture = current_gesture;
        current_gesture = GetGestureDetected();
        touch_position = GetTouchPosition(0);

        // In game timer
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
            // sword_progress += 2;
            // mpz_add_ui(skills.sword_fighting.current_level, skills.sword_fighting.current_level, 1);
            // sword_value = mpz_get_str(NULL, 10, skills.sword_fighting.current_level);
            // mpz_add_ui(skills.axe_fighting.current_level, skills.axe_fighting.current_level, 1);
            // axe_value = mpz_get_str(NULL, 10, skills.sword_fighting.current_level);
            time_frame = 0.0f;
        }

        if (IsKeyReleased(KEY_D)) {
            cube_position.z -= 2.1f;
            // if ((cube_position.x + 1.0f) > 2.1f * max_length - 2.0f) cube_position.x = 2.1f * max_length - 2.0f;
            // if ((player_position.x + 10) > playable_width + 200 - 20) player_position.x = playable_width + 200 - 20;
        }

        if (IsKeyReleased(KEY_A)) {
            cube_position.z += 2.1f;
            // if ((player_position.x - 10) < 200) player_position.x = 200;
        }

        if (IsKeyReleased(KEY_S)) {
            cube_position.x += 2.1f;
            // if ((player_position.y + 10) > playable_height + 50 - 20) player_position.y = playable_height + 50 - 20;
        }

        if (IsKeyReleased(KEY_W)) {
            cube_position.x -= 2.1f;
            // if ((player_position.y - 10) <= 50) player_position.y = 50;
        }
        if (enemy_cube_position.x + 2.0f < cube_position.x) enemy_cube_position.x += 0.05f;
        if (enemy_cube_position.x - 2.0f > cube_position.x) enemy_cube_position.x -= 0.05f;
        if (enemy_cube_position.z + 2.0f < cube_position.z) enemy_cube_position.z += 0.05f;
        if (enemy_cube_position.z - 2.0f > cube_position.z) enemy_cube_position.z -= 0.05f;

        camera3d.position.x = cube_position.x + 10.0f;
        camera3d.position.y = cube_position.y + 10.0f;
        camera3d.position.z = cube_position.z + 10.0f;
        camera3d.target = cube_position;

        // rpg_beging_ui_context(camera3d);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera3d);
            DrawCube(cube_position, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires(cube_position, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCube(enemy_cube_position, 2.0f, 2.0f, 2.0f, PURPLE);
            DrawCubeWires(enemy_cube_position, 2.0f, 2.0f, 2.0f, BLACK);
            for (int col = -max_length; col < max_length; ++col) {
                for (int row = -max_length; row < max_length; ++row) {
                    DrawCube((Vector3){(2.0f + padding) * row, -2.0f, (-2.0f - padding) * col}, 2.0f, 1.0f, 2.0f, DARKGREEN);
                    DrawCubeWires((Vector3){(2.0f + padding) * row, -2.0f, (-2.0f - padding) * col}, 2.0f, 1.0f, 2.0f, BLACK);
                }
            }
        EndMode3D();
            
            // DrawText(TextFormat("%s %s", skills.sword_fighting.name, sword_value), 200, 80, 20, BLACK);
            DrawRectangle(200, 120, 150, 20, LIGHTGRAY);
            DrawRectangle(200, 120, 150 * exp_percentage, 20, DARKGREEN);
            // DrawText(TextFormat("%s %s", skills.axe_fighting.name, axe_value), 200, 160, 20, BLACK);
        EndDrawing();
        // rpg_end_ui_context();
    }

    CloseWindow();
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}


common_return_t rpg_game_deinit(void) {
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_shutdown(rpg_game_state_t *gs) {
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

