#pragma once

#include <windows.h>

#include "Border.h"
#include "Level.h"

enum E_key_type
{
    EKT_left,
    EKT_right,
    EKT_space
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

    RECT rect;
    RECT prev_rect;
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

    static const int max_x_pos = C_level::x_offset + C_level::cell_width * C_level::width;
    static const int max_y_pos = 199 - C_ball::size;
    static const int circle_size = 7;

    HWND hwnd;
    HPEN pen_bg, pen_white, pen_dark_red, pen_blue, pen_cyan;
    HBRUSH brush_bg, brush_white, brush_dark_red, brush_blue, brush_cyan;

private:
    HPEN pen_white_fat, pen_light_red, pen_black;
    HBRUSH brush_light_red, brush_black;

    C_ball ball;
    C_level level;
    C_platform platform;
    C_border border;
};// end class

