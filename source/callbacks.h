#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "game_state.h"

typedef bool (*callback_t)(rpg_game_state_t *gs);

bool
buy_upgrade(rpg_game_state_t *gs);

#endif // CALLBACKS_H_

