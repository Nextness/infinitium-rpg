#include <raylib.h>
#include <stdbool.h>

typedef struct {
    // Config functions
    void (*init_window)(int, int, const char *);
    void (*set_target_fps)(int);
    bool (*window_should_close)(void);
    float (*get_frame_time)(void);
    void (*begin_drawing)(void);
    void (*clear_background)(Color);
    void (*end_drawing)(void);
    void (*close_window)(void);

    // UI Object functions
    void (*draw_rectangle)(int, int, int, int, Color);

} wrapper_raylib_t;

#define rl_config_functions                   \
    .init_window = InitWindow,                \
    .set_target_fps = SetTargetFPS,           \
    .window_should_close = WindowShouldClose, \
    .get_frame_time = GetFrameTime,           \
    .begin_drawing = BeginDrawing,            \
    .clear_background = ClearBackground,      \
    .end_drawing = EndDrawing,                \
    .close_window = CloseWindow,

#define rl_ui_object_functions       \
    .draw_rectangle = DrawRectangle,

#define init_raylib_wrapper(rl) \
wrapper_raylib_t rl = {         \
    rl_config_functions         \
    rl_ui_object_functions      \
};

