#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <stdbool.h>

typedef struct rpg_game_state_t rpg_game_state_t;

typedef bool (*callback_t)(rpg_game_state_t *gs);

bool
buy_upgrade_1_adv(rpg_game_state_t *gs);

bool
buy_upgrade_2_adv(rpg_game_state_t *gs);

bool
buy_upgrade_3_adv(rpg_game_state_t *gs);

bool
on_change_upgrade_1_adv(rpg_game_state_t *gs);

bool
on_change_upgrade_2_adv(rpg_game_state_t *gs);

bool
on_change_upgrade_3_adv(rpg_game_state_t *gs);

#endif // CALLBACKS_H_

