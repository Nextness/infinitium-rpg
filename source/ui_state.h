#ifndef UI_STATE_H_
#define UI_STATE_H_

#include <raylib.h>

#include "utils_common.h"
#include "wrapper.h"
#include "ui_components.h"

typedef struct {
    int current_gesture;
    rl_vector2_t mouse_position;
    uic_button_config_t buttons[8];
} rpg_ui_state_t;

#define using_rpg_ui_state_t(gs, ui_state) rpg_ui_state_t* (ui_state) = &(gs)->ui_state

common_return_t
ui_state_init_config(rpg_ui_state_t *ui_state);

#endif // UI_STATE_H_

