#ifndef UI_COMPONENTS_H_
#define UI_COMPONENTS_H_

#include <raylib.h>

#include "utils_common.h"
#include "callbacks.h"
#include "wrapper.h"

typedef struct {
    rl_rectangle_t button_area;
    rl_color_t button_color;
    int border_width;
    rl_color_t border_color;
    char *button_text;
    rl_color_t button_text_color;
    int button_text_size;
    char button_id[16];
    callback_t callback;
    callback_t on_change;
} uic_button_config_t;

struct rpg_game_state_t;

common_return_t
uic_button(rpg_game_state_t *gs inout(), int index);

#endif // UI_COMPONENTS_H_

