#ifndef UI_COMPONENTS_H_
#define UI_COMPONENTS_H_

#include <raylib.h>
#include "utils_common.h"
#include "game_state.h"
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
} uic_button_config_t;

common_return_t
uic_button(rl_vector2_t mouse_position in(), uic_button_config_t button in(),
           rpg_game_state_t *gs inout(), callback_t callback in());

#endif // UI_COMPONENTS_H_

