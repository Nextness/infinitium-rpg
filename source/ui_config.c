#include "ui_config.h"
#include "utils_common.h"

common_return_t
ui_init_set_config(rpg_ui_config_t *ui_config inout())
{
    ui_config->screen_width = 800;
    ui_config->screen_height = 450;
    return common_set_return(COMMON_OK, NULL, NULL);
}

