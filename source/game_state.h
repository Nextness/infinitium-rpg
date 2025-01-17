#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <raylib.h>

#include "ui_config.h"
#include "ui_state.h"
#include "player.h"

typedef struct rpg_game_state_t {
    float time_frame;
    rpg_ui_state_t ui_state;
    rpg_player_t player;
} rpg_game_state_t;

#endif // GAME_STATE_H_

