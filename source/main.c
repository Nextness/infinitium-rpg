#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

#include "rpg_common.h"
#include "ui_config.c"
// #include "player.c"

#define FRAME_RATE 60
#define MAX_BUILDINGS 100

typedef struct {
    rpg_ui_config_t ui_config;
//    rpg_player_t player;
} rpg_game_state_t;

void rpg_beging_ui_context(Camera2D camera) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
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
    // (void)gs;
    const int screen_width = 1280; 
    const int screen_height = 720; 

    InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");

    Rectangle player = {400, 280, 40, 40};
    Rectangle buildings[MAX_BUILDINGS] = {0};
    Color build_colors[MAX_BUILDINGS] = {0};

    int spacing = 0;

    for (int i = 0; i < MAX_BUILDINGS; i++) {
        buildings[i].width = (float) GetRandomValue(50, 200);
        buildings[i].height = (float) GetRandomValue(100, 800);
        buildings[i].y = screen_height - 400.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;
        spacing += (int) buildings[i].width;
        build_colors[i] = (Color) {
            .r = GetRandomValue(200, 240),
            .g = GetRandomValue(200, 240),
            .b = GetRandomValue(200, 250),
            .a = 255
        };
    }

    Camera2D camera = {0};
    camera.target = (Vector2) {
        .x = player.x + 20.0f,
        .y = player.y + 20.0f
    };
    camera.offset = (Vector2) {
        .x = screen_width/2.0f,
        .y = screen_height/2.0f
    };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(FRAME_RATE);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) player.x += 5;
        else if (IsKeyDown(KEY_LEFT)) player.x += -5;

        camera.target = (Vector2) { player.x + 20, player.y + 20};

        camera.zoom += (float) GetMouseWheelMove()*0.05f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        rpg_beging_ui_context(camera);
            DrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);
            for (int i = 0; i < MAX_BUILDINGS; i++) {
                DrawRectangleRec(buildings[i], build_colors[i]);
            }
            DrawRectangleRec(player, RED);
            DrawLine((int) camera.target.x, -screen_height*10, (int) camera.target.x, screen_height*10, GREEN);
            DrawLine(-screen_width*10, (int) camera.target.y, screen_width*10, (int) camera.target.y, GREEN);
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
    // free(gs);
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

