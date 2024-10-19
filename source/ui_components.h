#ifndef UI_COMPONENTS_H_
#define UI_COMPONENTS_H_

#include <raylib.h>
#include "utils_common.h"
#include "game_state.h"
#include "callbacks.h"
#include "wrapper.h"

typedef struct {
    Rectangle button_area;
    Color button_color;
    int border_width;
    Color border_color;
    char *button_text;
} uic_button_config_t;

common_return_t
uic_button(rl_vector2_t mouse_position in(), uic_button_config_t button in(),
           rpg_game_state_t *gs inout(), callback_t callback in());

#endif // UI_COMPONENTS_H_

