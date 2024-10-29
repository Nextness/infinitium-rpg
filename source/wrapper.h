#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <raylib.h>

// Config/Init/End functions
#define rl_set_trace_log_level SetTraceLogLevel
#define rl_init_window InitWindow
#define rl_set_target_fps SetTargetFPS
#define rl_window_should_close WindowShouldClose
#define rl_begin_drawing BeginDrawing
#define rl_clear_background ClearBackground
#define rl_end_drawing EndDrawing
#define rl_close_window CloseWindow
#define rl_set_config_flags SetConfigFlags

// Utils
#define rl_get_frame_time GetFrameTime
#define rl_color_from_hsv ColorFromHSV

// UI Object functions
#define rl_draw_rectangle DrawRectangle
#define rl_draw_rectangle_rec DrawRectangleRec
#define rl_draw_rectangle_rounded DrawRectangleRounded
#define rl_draw_text DrawText

// Interactions
#define rl_get_gesture_detected GetGestureDetected
#define rl_get_touch_position GetTouchPosition
#define rl_check_collision_point_rec CheckCollisionPointRec
#define rl_is_mouse_button_pressed IsMouseButtonPressed
#define rl_is_mouse_button_released IsMouseButtonReleased

// Structs
#define rl_color_t Color
#define rl_vector2_t Vector2
#define rl_vector3_t Vector3
#define rl_rectangle_t Rectangle

#endif // WRAPPER_H_

