#include "ui_config.h"
#include "utils_common.h"

common_return_t
rpg_init_set_ui_config_adv(int screen_width in(), int screen_height in(), rpg_ui_config_t *ui_config inout())
{
    ui_config->screen_width = screen_width;
    ui_config->screen_height = screen_height;
    return common_set_return(COMMON_OK, NULL);
}

void rpg_init_set_ui_config(rpg_ui_config_t *ui_config, int screen_width, int screen_height) {
    ui_config->screen_width = screen_width;
    ui_config->screen_height = screen_height;
}

