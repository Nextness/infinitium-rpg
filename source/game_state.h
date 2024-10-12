#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "ui_config.h"
#include "player.h"

typedef struct {
    float time_frame;
    rpg_ui_config_t ui_config;
    rpg_player_t player;
} rpg_game_state_t;

#endif // GAME_STATE_H_

