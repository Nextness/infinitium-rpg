#include <raylib.h>
#include <stdio.h>

#if DEVELOPER_BUILD
#    include <assert.h>
#endif

#define _Nullable
#define _Nonnull
#define _In
#define _Out

#define type
#define proc
#define macro

enum Result_e type {
    OK,
    ERROR,
};

struct Result type {
    enum Result_e res;
    char * _Nullable dev_error_msg;
    char * _Nullable user_error_msg;
};

#define set_result(res, dev_error_msg, user_error_msg) macro \
    (struct Result) { (res), (dev_error_msg), (user_error_msg) }

#define get_result(Result)         macro (Result).res
#define get_error(Result)          macro (Result).res
#define get_dev_error_msg(Result)  macro (Result).dev_error_msg
#define get_user_error_msg(Result) macro (Result).user_error_msg


struct UI_Config type {
    int screen_height;
    int screen_width;
    const char * _Nonnull window_name;
};

typedef struct Button Button;
typedef bool (*button_callback)(struct Button * _Nonnull _In _Out);

struct Button type {
    struct Display type {
        char * _Nonnull text;
        char * _Nullable afix;
        char * _Nullable prefix;
        char * _Nullable sufix;
    } display;
    struct Style type {
        Vector2 position;
        float width;
        float height;
        Color color;
        Color text_color;
        int text_size;
        int border_width;
        Color border_color;
    } style;
    struct Callbacks type {
        button_callback on_idle;
        button_callback on_hover;
        button_callback on_click;
    } callbacks;
};


struct Result simple_button(struct Button button) proc {
    struct Result result = set_result(OK, NULL, NULL);
    struct Vector3 btn_color_hsv = ColorToHSV(button.style.color);
    struct Rectangle area = {
        .x = button.style.position.x,
        .y = button.style.position.y,
        .width = button.style.width,
        .height = button.style.height,
    };

    // Iddle button
    if (!CheckCollisionPointRec(GetTouchPosition(0), area)) {
        btn_color_hsv.z *= 0.5f;
        goto render_button;
    }

    // Hover button
    if (!IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        btn_color_hsv.z *= 0.6f;
        goto render_button;
    }

    // TODO: Implement onclick callback

render_button:
    button.style.color = ColorFromHSV(btn_color_hsv.x, btn_color_hsv.y, btn_color_hsv.z);

    int text_width = MeasureText(button.display.text, button.style.text_size);
    Rectangle border_area = {
        .x = button.style.position.x - button.style.border_width,
        .y = button.style.position.y - button.style.border_width,
        .width = button.style.width + 2 * button.style.border_width,
        .height = button.style.height + 2 * button.style.border_width,
    };

    DrawRectangleRounded(border_area, 0.2f, 30, button.style.border_color);
    DrawRectangleRounded(area, 0.2f, 20, button.style.color);
    DrawText(button.display.text,
             button.style.position.x + (button.style.width - text_width) / 2,
             button.style.position.y + (button.style.height - button.style.text_size) / 2,
             button.style.text_size,
             button.style.text_color);

    return result;
}

struct Result main_game_entry(void) proc {
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
    };

    InitWindow(ui_config.screen_height, ui_config.screen_width, ui_config.window_name);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        simple_button(button);
        EndDrawing();
    }
    CloseWindow();

    return result;
}

int main() proc {
    struct Result result = main_game_entry();
    return get_result(result);
}
