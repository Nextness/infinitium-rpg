#include <raylib.h>

#include "game_state.h"
#include "ui_state.h"
#include "utils_common.h"
#include "ui_components.h"
#include "callbacks.h"
#include "wrapper.h"

// common_return_t
// uic_button(rl_vector2_t mouse_position in(), uic_button_config_t button in(),
           // rpg_game_state_t *gs inout(), callback_t on_click in(), callback_t on_change in())
common_return_t
uic_button(rpg_game_state_t *gs inout(), int index)
{
    using_rpg_ui_state_t(gs, ui_state);
    uic_button_config_t *button = &gs->ui_state.buttons[index];
    rl_vector3_t btn_color_hsv = ColorToHSV(button->button_color);
    rl_vector3_t border_color_hsv = ColorToHSV(button->border_color);

    // Idle button state
    if (!rl_check_collision_point_rec(ui_state->mouse_position, button->button_area)) {
        btn_color_hsv.z = button->on_change(gs) ? 0.8f : 0.5f;
        border_color_hsv.z = button->on_change(gs) ? 0.8f : 0.5f;
        goto render_button_l;
    }

    // Hover button state
    if (!rl_is_mouse_button_released(MOUSE_BUTTON_LEFT)) {
        btn_color_hsv.z = button->on_change(gs) ? 0.9f : 0.6f;
        border_color_hsv.z = button->on_change(gs) ? 0.9f : 0.6f;
        goto render_button_l;
    }

    // On click button state
    if (!button->callback(gs)) {
        goto render_button_l;
    }

render_button_l:
    button->button_color = rl_color_from_hsv(btn_color_hsv.x, btn_color_hsv.y, btn_color_hsv.z);
    button->border_color = rl_color_from_hsv(border_color_hsv.x, border_color_hsv.y,
                                            border_color_hsv.z);

    int text_width = MeasureText(button->button_text, button->button_text_size);
    rl_rectangle_t button_rect = {
        .x = button->button_area.x - button->border_width,
        .y = button->button_area.y - button->border_width,
        .width = button->button_area.width + 2 * button->border_width,
        .height = button->button_area.height + 2 * button->border_width
    };

    rl_draw_rectangle_rounded(button_rect, 0.2f, 30, button->border_color);
    rl_draw_rectangle_rounded(button->button_area, 0.2f, 20, button->button_color);
    rl_draw_text(button->button_text,
             button->button_area.x + (button->button_area.width - text_width) / 2,
             button->button_area.y + button->button_area.height / 2,
             button->button_text_size,
             button->button_text_color);

    return common_set_return(COMMON_OK, NULL, NULL);
}

