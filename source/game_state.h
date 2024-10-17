#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <raylib.h>

#include "ui_config.h"
#include "player.h"

typedef struct {
    int current_gesture;
    Vector2 mouse_position;
} rpg_ui_state_t;

typedef struct {
    float time_frame;
    rpg_ui_config_t ui_config;
    rpg_ui_state_t ui_state;
    rpg_player_t player;
} rpg_game_state_t;

#define using_rpg_ui_state_t(gs, ui_state) rpg_ui_state_t* (ui_state) = &(gs)->ui_state

#endif // GAME_STATE_H_

