#include "game_state.h"
#include "utils_common.h"
#include "ui_components.h"
#include "callbacks.h"
#include "wrapper.h"
#include <raylib.h>

common_return_t
uic_button(rl_vector2_t mouse_position in(), uic_button_config_t button in(),
           rpg_game_state_t *gs inout(), callback_t callback in())
{
    rl_vector3_t color_hsv = ColorToHSV(button.button_color);
    float default_saturation = color_hsv.z;

    // Not on top of the button or hovering, just draw the button.
    if (!rl_check_collision_point_rec(mouse_position, button.button_area)) {
        color_hsv.z = default_saturation;
        goto draw_button_l;
    }

     // Only hovering the button
     if (!rl_is_mouse_button_released(MOUSE_BUTTON_LEFT)) {
        color_hsv.z = 0.70f;
        goto draw_button_l;
     }

    // On top and clicking
    if (!callback(gs)) {
        goto draw_button_l;
    }

draw_button_l:
    button.button_color = rl_color_from_hsv(color_hsv.x, color_hsv.y, color_hsv.z);
    rl_draw_rectangle_rounded((rl_rectangle_t) {
        button.button_area.x - button.border_width, 
        button.button_area.y - button.border_width, 
        button.button_area.width + 2 * button.border_width,
        button.button_area.height + 2 * button.border_width
    },0.2f, 30, button.border_color);
    rl_draw_rectangle_rounded(button.button_area, 0.2f, 20, button.button_color);
    int text_width = MeasureText(button.button_text, button.button_text_size);
    rl_draw_text(button.button_text,
             button.button_area.x + (button.button_area.width - text_width) / 2,
             button.button_area.y + button.button_area.height / 2,
             button.button_text_size,
             button.button_text_color);
    return common_set_return(COMMON_OK, NULL);
}

