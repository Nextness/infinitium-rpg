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

void rpg_beging_ui_context(/*Camera2D camera*/void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // BeginMode2D(camera);
}

void rpg_end_ui_context(void) {
    EndMode2D();
    EndDrawing();
}

common_return_t rpg_game_setup(rpg_game_state_t *gs) {
    (void)gs;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_init(rpg_game_state_t *gs) {
    gs->ui_config.screen_width = 1280;
    gs->ui_config.screen_height = 720;
    return COMMON_SET_RETURN(COMMON_OK, NULL);
}

common_return_t rpg_game_running(rpg_game_state_t *gs) {
    InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    SetTargetFPS(FRAME_RATE);
    rpg_common_skills_t skills = rpg_common_starter_skills();

    float time_frame = 0.0f;
    Vector2 touch_position = { 0, 0 };

    float pos_x = 200;
    float pos_y = 50;
    float playable_width = GET_SW(gs) - 400.0f;
    float playable_height = GET_SH(gs) - 80.0f;
    Rectangle touch_area = { pos_x, pos_y, playable_width, playable_height};

    Vector2 enemy_position = {pos_x, pos_y};
    Vector2 player_position = {pos_x + (playable_width / 2) , pos_y + (playable_height / 2)};

    int current_gesture = GESTURE_NONE;
    int last_gesture = GESTURE_NONE;
    // char *sword_value = "10";
    // char *axe_value = "10";
    // int sword_progress = 0;
    // char *sword_experience = "100";

    while (!WindowShouldClose()) {
        last_gesture = current_gesture;
        current_gesture = GetGestureDetected();
        touch_position = GetTouchPosition(0);

        // In game timer
        if ((time_frame += GetFrameTime()) >= 1.0f) {
            // sword_progress += 2;
            // mpz_add_ui(skills.sword_fighting.current_level, skills.sword_fighting.current_level, 1);
            // sword_value = mpz_get_str(NULL, 10, skills.sword_fighting.current_level);
            // mpz_add_ui(skills.axe_fighting.current_level, skills.axe_fighting.current_level, 1);
            // axe_value = mpz_get_str(NULL, 10, skills.sword_fighting.current_level);
            time_frame = 0.0f;
        }

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player_position.x += 5.0f;
            if ((player_position.x + 10) > playable_width + 200 - 20) player_position.x = playable_width + 200 - 20;
        }

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player_position.x -= 5.0f;
            if ((player_position.x - 10) < 200) player_position.x = 200;
        }

        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            player_position.y += 5.0f;
            if ((player_position.y + 10) > playable_height + 50 - 20) player_position.y = playable_height + 50 - 20;
        }

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            player_position.y -= 5.0f;
            if ((player_position.y - 10) <= 50) player_position.y = 50;
        }

        if (enemy_position.x + 25 < player_position.x) enemy_position.x += 1;
        if (enemy_position.x - 25 > player_position.x) enemy_position.x -= 1;
        if (enemy_position.y + 25 < player_position.y) enemy_position.y += 1;
        if (enemy_position.y - 25 > player_position.y) enemy_position.y -= 1;

        rpg_beging_ui_context();
            DrawRectangleRec(touch_area, DARKGRAY);
            DrawRectangle(205, 55, gs->ui_config.screen_width - 410, gs->ui_config.screen_height - 90, RAYWHITE);
            if (CheckCollisionPointRec(touch_position, touch_area) && (current_gesture != GESTURE_NONE)) {
                DrawCircleV(touch_position, 30, MAROON);
            }
            DrawRectangle(player_position.x, player_position.y, 20, 20, DARKGREEN);
            DrawRectangle(enemy_position.x, enemy_position.y, 20, 20, RED);

            // DrawText(TextFormat("%s %s", skills.sword_fighting.name, sword_value), 200, 80, 20, BLACK);
            // DrawRectangle(200, 120, 200, 20, LIGHTGRAY);
            // DrawRectangle(200, 120, sword_progress, 20, DARKGREEN);
            // DrawText(TextFormat("%s %s", skills.axe_fighting.name, axe_value), 200, 160, 20, BLACK);
        rpg_end_ui_context();
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

