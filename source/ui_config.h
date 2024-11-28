#ifndef UI_CONFIG_H_
#define UI_CONFIG_H_

#include "utils_common.h"

typedef struct {
    int screen_width;
    int screen_height;
} ui_config_t;

common_return_t
ui_init_set_config(ui_config_t *ui_config inout());


// typedef struct {} 
//  
// ui_get_buttons();


#endif // UI_CONFIG_H_

