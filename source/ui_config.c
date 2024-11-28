#include "ui_config.h"
#include "utils_common.h"
#include <raylib.h>

common_return_t
ui_init_set_config(rpg_ui_config_t *ui_config inout())
{
    int display = GetCurrentMonitor();
    ui_config->screen_width = GetMonitorWidth(display);
    ui_config->screen_height = GetMonitorHeight(display);
    return common_set_return(COMMON_OK, NULL, NULL);
}

