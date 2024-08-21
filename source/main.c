#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

#include "ui_config.c"
#include "player.c"

#define FRAME_RATE 60

typedef struct {
    rpg_ui_config_t ui_config;
    rpg_player_t player;
} rpg_game_state_t;

rpg_game_state_t * rpg_game_setup()
{
    // Allocate game state
    rpg_game_state_t *gs = (rpg_game_state_t *) malloc(sizeof(rpg_game_state_t));
    if (gs == NULL) {
        printf("Buy more RAM for game state allocation...\n");
        return NULL;
    }

    // Set up user interface configuration
    rpg_init_set_ui_config(&gs->ui_config, 1280, 720);

    return gs;
}

void rpg_game_init(void)
{
}

void rpg_game_running()
{
    InitWindow(1280, 720, "First Window");
    SetTargetFPS(FRAME_RATE);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
}

void rpg_game_deinit(void)
{
}

void rpg_game_shutdown(rpg_game_state_t *gs)
{
    free(gs);
}

int main(void)
{
    rpg_game_state_t *gs = NULL;
    if ((gs = rpg_game_setup()) == NULL) {
        exit(1);
    }
    rpg_game_init();
    rpg_game_running();
    rpg_game_deinit();
    rpg_game_shutdown(gs);
}

