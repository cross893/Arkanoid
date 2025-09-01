#pragma once

#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum E_key_type
{
    EKT_left,
    EKT_right,
    EKT_space
};

enum E_brick_type
{// Создане перечисления для типа кирпичей
    EBT_none,
    EBT_light_red,
    EBT_cyan
};

enum E_letter_type
{
    ELT_none,
    ELT_o
};

const int timer_id = WM_USER + 1;

//------------------------------------------------------------------------------------------------------------
class C_engine;

//------------------------------------------------------------------------------------------------------------
class C_ball
{
public:
    C_ball();

    void F_draw(HDC hdc, RECT& paint_area, C_engine* engine);
    void F_move(C_engine* engine);

    static const int   ball_size = 4;

    double      ball_direction;

private:
    int         ball_x_pos;
    int         ball_y_pos;
    double      ball_speed;

    RECT        ball_rect;
    RECT        prev_ball_rect;
};

//------------------------------------------------------------------------------------------------------------
class C_engine
{
public:
    C_engine();

    void F_init_engine  (HWND hwnd);
    void F_draw_frame   (HDC hdc, RECT &paint_area);
    int F_on_key_down   (E_key_type key_type);
    int F_on_timer      ();
    void F_check_level_brick_hit(int& next_y_pos);

    static const int   level_width = 12;
    static const int   level_height = 14;
    static const int   level_x_offset = 8;
    static const int   level_y_offset = 6;
    static const int   cell_width = 16;
    static const int   cell_height = 8;
    static const int   max_x_pos = level_x_offset + cell_width * level_width;
    static const int   max_y_pos = 199 - C_ball::ball_size;
    static const int   y_pos_platform = 185;
    static const int   border_x_offset = 6;
    static const int   border_y_offset = 4;
    static const int   global_scale = 3;

    int         x_pos_platform;
    int         x_step_platform;
    int         platform_width;

    HWND        hwnd;
    HPEN        pen_bg, pen_white;
    HBRUSH      brush_bg, brush_white;

private:
    void F_create_pen_brush         (unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
    void F_redraw_platform          ();
    void F_draw_brick               (HDC hdc, int x, int y, E_brick_type brick_type);
    void F_set_brick_letter_color   (bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush);
    void F_draw_brick_letter        (HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step);
    void F_draw_level               (HDC hdc);
    void F_draw_platform            (HDC hdc, int x, int y);
    void F_draw_border              (HDC hdc, int x, int y, bool top_border);
    void F_draw_bounds              (HDC hdc, RECT &paint_area);

    static const int   brick_width = 15;
    static const int   brick_height = 7;
    static const int   circle_size = 7;
    static const int   platform_height = 7;

    int         inner_width;

    HPEN        pen_white_fat, pen_black, pen_light_red, pen_cyan, pen_dark_red, pen_blue;
    HBRUSH      brush_black, brush_light_red, brush_cyan, brush_dark_red, brush_blue;

    RECT        platform_rect, prev_platform_rect;
    RECT        level_rect;

    C_ball ball;
}; // end class

