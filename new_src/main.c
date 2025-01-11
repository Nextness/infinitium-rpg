#include <raylib.h>
#include <stdio.h>
#include <gmp.h>

#if DEVELOPER_BUILD
#    include <assert.h>
#endif

#define _Nullable
#define _Nonnull
#define _In
#define _Out

enum Result_e {
    OK,
    ERROR,
};

struct Result {
    enum Result_e res;
    char * _Nullable dev_error_msg;
    char * _Nullable user_error_msg;
};

#define set_result(res, dev_error_msg, user_error_msg) \
    (struct Result) { (res), (dev_error_msg), (user_error_msg) }

#define get_result(Result) (Result).res
#define get_error(Result) (Result).res
#define get_dev_error_msg(Result) (Result).dev_error_msg
#define get_user_error_msg(Result) (Result).user_error_msg

#define start_renderer(keep_rendering) while ((keep_rendering)) {
#define end_renderer() }

#define new_mpf_from_struct(variable_name, initial_value) \
    mpf_init((variable_name));                            \
    mpf_set_str((variable_name), (initial_value), 10)

#define new_mpf(variable_name, initial_value)         \
    mpf_t (variable_name);                            \
    mpf_init((variable_name));                        \
    mpf_set_str((variable_name), (initial_value), 10)

#define clear_mpf(variable_name)             \
    mpf_set_str((variable_name), "0.0", 10); \
    mpf_clear((variable_name))

struct UI_Config {
    int screen_height;
    int screen_width;
    const char * _Nonnull window_name;
};

struct Player {
    mpf_t experience;
};

typedef struct Button Button;
typedef bool (*button_callback)(
    struct Button * _Nonnull _In _Out,
    struct Player * _Nonnull _In _Out
);

struct Button {
    struct Display {
        char * _Nonnull text;
        char * _Nullable afix;
        char * _Nullable prefix;
        char * _Nullable sufix;
    } display;
    struct Style {
        Vector2 position;
        float width;
        float height;
        Color color;
        Color text_color;
        int text_size;
        int border_width;
        Color border_color;
    } style;
    struct Animations {
        float color_fade_furation;
    } animations;
    struct Callbacks {
        button_callback on_idle;
        button_callback on_hover;
        button_callback on_able_to_buy;
        button_callback on_click;
    } callbacks;
};

bool on_idle(
    struct Button * _Nonnull _In _Out button,
    struct Player * _Nonnull _In _Out player
) {
    (void)player;
    return !CheckCollisionPointRec(
        GetTouchPosition(0),
        (struct Rectangle) {
            .x = button->style.position.x,
            .y = button->style.position.y,
            .width = button->style.width,
            .height = button->style.height
    });
}

bool on_hover(
    struct Button * _Nonnull _In _Out button,
    struct Player * _Nonnull _In _Out player
) {
    (void)button;
    (void)player;
    return !IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

bool on_able_to_buy(
    struct Button * _Nonnull _In _Out button,
    struct Player * _Nonnull _In _Out player
) {
    (void)button;
    new_mpf(cost, "1.0");
    new_mpf(tmp, "100.0");

    bool result = mpf_cmp(player->experience, cost) >= 0;
    mpf_clears(cost, tmp, NULL);

    return result;
}

struct Result simple_button(
    struct Button * _Nonnull _In _Out button,
    struct Player * _Nonnull _In _Out player
) {
    struct Result result = set_result(OK, NULL, NULL);
    struct Vector3 btn_color_hsv = ColorToHSV(button->style.color);

    // Iddle state
    if (button->callbacks.on_idle(button, player)) {
        btn_color_hsv.z = button->callbacks.on_able_to_buy(button, player) ? 0.8f : 0.5f;
        goto render_button;
    }

    // Hover state
    if (button->callbacks.on_hover(button, player)) {
        btn_color_hsv.z = button->callbacks.on_able_to_buy(button, player) ? 0.9f : 0.6f;
        goto render_button;
    }

    // TODO: Implement onclick state

render_button:
    button->style.color = ColorFromHSV(btn_color_hsv.x, btn_color_hsv.y, btn_color_hsv.z);

    int text_width = MeasureText(button->display.text, button->style.text_size);
    Rectangle border_area = {
        .x = button->style.position.x - button->style.border_width,
        .y = button->style.position.y - button->style.border_width,
        .width = button->style.width + 2 * button->style.border_width,
        .height = button->style.height + 2 * button->style.border_width,
    };

    DrawRectangleRounded(border_area, 0.2f, 30, button->style.border_color);
    DrawRectangleRounded(
        (struct Rectangle) {
            .x = button->style.position.x,
            .y = button->style.position.y,
            .width = button->style.width,
            .height = button->style.height,
        }, 0.2f, 20, button->style.color);

    DrawText(button->display.text,
             button->style.position.x + (button->style.width - text_width) / 2,
             button->style.position.y + (button->style.height - button->style.text_size) / 2,
             button->style.text_size,
             button->style.text_color);

    return result;
}

struct Result main_game_entry(void) {
    struct Result result = set_result(OK, NULL, NULL);
    struct UI_Config ui_config = {1200, 800, "Infinitium RPG - Incremental Game"};

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    struct Button button = {
        .display = {
            .text = "First Upgrade"
        },
        .style = {
            .position = {100, 200},
            .width = 100,
            .height = 40,
            .color = LIGHTGRAY,
            .text_color = BLUE,
            .text_size = 24,
            .border_color = BLACK,
            .border_width = 4.0f,
        },
        .callbacks = {
            .on_idle = on_idle,
            .on_hover = on_hover,
            .on_able_to_buy = on_able_to_buy,
        },
    };

    struct Player player;
    new_mpf_from_struct(player.experience, "0.0");
    float passed_time = 0.0f;
    const int max_length = 1024;
    char experience_str[max_length];
    sprintf(experience_str, "0.00");

    InitWindow(ui_config.screen_height, ui_config.screen_width, ui_config.window_name);
        start_renderer(!WindowShouldClose());
            passed_time += GetFrameTime();
            if (passed_time >= 1.0f) {
                mpf_add_ui(player.experience, player.experience, 1);
                gmp_snprintf(experience_str, max_length, "%.*Ff", 2, player.experience);
                passed_time = 0.0f;
            }
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(experience_str, 300, 300, 30, BLACK);
            simple_button(&button, &player);
            EndDrawing();
        end_renderer();
    CloseWindow();

    clear_mpf(player.experience);
    return result;
}

int main() {
    struct Result result = main_game_entry();
    return get_result(result);
}
