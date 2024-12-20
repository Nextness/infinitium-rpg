#include <raylib.h>

#include "utils_common.h"
#include "ui_state.h"
#include "callbacks.h"
#include "wrapper.h"

common_return_t
ui_state_init_config(rpg_ui_state_t *ui_state)
{
    ui_state->current_gesture = rl_get_gesture_detected();
    ui_state->mouse_position = rl_get_touch_position(0);

    ui_state->buttons[0] = (uic_button_config_t) {
        .button_area = (rl_rectangle_t) {100, 100, 200, 50},
        .button_color = BLUE,
        .border_width = 4,
        .border_color = BLACK,
        .button_text = "First Upgrade",
        .button_text_color = BLACK,
        .button_text_size = 16,
        .button_id = "upgd-1",
        .on_change = on_change_upgrade_1_adv,
        .callback = buy_upgrade_1_adv,
    };
    ui_state->buttons[1] = (uic_button_config_t) {
        .button_area = (rl_rectangle_t) {100, 200, 200, 50},
        .button_color = RED,
        .border_width = 4,
        .border_color = BLACK,
        .button_text = "Second Upgrade",
        .button_text_color = BLACK,
        .button_text_size = 16,
        .button_id = "upgd-2",
        .on_change = on_change_upgrade_2_adv,
        .callback = buy_upgrade_2_adv,
    };
    ui_state->buttons[2] = (uic_button_config_t) {
        .button_area = (rl_rectangle_t) {100, 300, 200, 50},
        .button_color = RED,
        .border_width = 4,
        .border_color = BLACK,
        .button_text = "Third Upgrade",
        .button_text_color = BLACK,
        .button_text_size = 16,
        .button_id = "upgd-3",
        .on_change = on_change_upgrade_3_adv,
        .callback = buy_upgrade_3_adv,
    };

    return common_set_return(COMMON_OK, NULL, NULL);
}
