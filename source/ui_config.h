#ifndef UI_CONFIG_H_
#define UI_CONFIG_H_

#include "utils_common.h"

typedef struct {
    int screen_width;
    int screen_height;
} rpg_ui_config_t;

common_return_t
rpg_init_set_ui_config_adv(int screen_width in(), int screen_height in(), rpg_ui_config_t *ui_config inout());

#endif // UI_CONFIG_H_
