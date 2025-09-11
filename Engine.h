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
class C_level;
class C_platform;

//------------------------------------------------------------------------------------------------------------
class C_ball
{
public:
    C_ball();

    void F_draw(HDC hdc, RECT &paint_area, C_engine *engine);
    void F_move(C_engine *engine, C_level *level, C_platform* platform);

    static const int   size = 4;

    double      direction;

private:
    int         x_pos;
    int         y_pos;
    double      speed;

    RECT        rect;
    RECT        prev_rect;
};// end class

//------------------------------------------------------------------------------------------------------------
class C_level
{
public:
    void F_init();
    void F_check_level_brick_hit(int& next_y_pos, double& direction);
    void F_draw(HDC hdc, RECT& paint_area);

    static const int   width = 12;
    static const int   height = 14;
    static const int   x_offset = 8;
    static const int   y_offset = 6;
    static const int   cell_width = 16;
    static const int   cell_height = 8;

private:
    void F_draw_brick(HDC hdc, int x, int y, E_brick_type brick_type);
    void F_set_brick_letter_color(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void F_draw_brick_letter(HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step);

    static const int   brick_width = 15;
    static const int   brick_height = 7;

    HPEN pen_white_fat, pen_light_red, pen_cyan;
    HBRUSH brush_light_red, brush_cyan;

    RECT rect;
};// end class

//------------------------------------------------------------------------------------------------------------
class C_platform
{
public:
    C_platform();

    void F_init();
    void F_redraw(C_engine* engine);
    void F_draw(HDC hdc, C_engine* engine, RECT& paint_area);


    static const int y_pos = 185;

    int x_pos;
    int x_step;
    int width;

private:
    static const int height = 7;

    int inner_width;

    RECT rect, prev_rect;
};// end class

//------------------------------------------------------------------------------------------------------------
class C_border
{
public:
    void F_draw(HDC hdc, RECT& paint_area, C_engine *engine);

    static const int x_offset = 6;
    static const int y_offset = 4;

private:
    void F_draw_element(HDC hdc, int x, int y, bool top_border, C_engine* engine);
};// end class

//------------------------------------------------------------------------------------------------------------
class C_engine
{
public:
    C_engine();

    void F_init(HWND hwnd);
    void F_draw_frame(HDC hdc, RECT &paint_area);
    int F_on_key_down(E_key_type key_type);
    int F_on_timer();
    static void F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

    static const int max_x_pos = C_level::x_offset + C_level::cell_width * C_level::width;
    static const int max_y_pos = 199 - C_ball::size;
    static const int global_scale = 3;
    static const int circle_size = 7;

    HWND        hwnd;
    HPEN        pen_bg, pen_white, pen_dark_red, pen_blue, pen_cyan;
    HBRUSH      brush_bg, brush_white, brush_dark_red, brush_blue, brush_cyan;

private:
    HPEN        pen_white_fat, pen_light_red, pen_black;
    HBRUSH      brush_light_red, brush_black;

    C_ball ball;
    C_level level;
    C_platform platform;
    C_border border;
};// end class

