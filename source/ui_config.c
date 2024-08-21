
typedef struct {
    int screen_width, screen_height;
} rpg_ui_config_t;

void rpg_init_set_ui_config(rpg_ui_config_t *ui_config, int screen_width, int screen_height) {
    ui_config->screen_width = screen_width;
    ui_config->screen_height = screen_height;
}

