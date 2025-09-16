#pragma once

#include "Config.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
class C_ball
{
public:
    C_ball();

    void F_draw(HDC hdc, RECT &paint_area, HPEN pen_bg, HBRUSH brush_bg, HPEN pen_white, HBRUSH brush_white);
    void F_move(HWND hwnd, C_level* level, int platform_x_pos, int platform_width);

    double      direction;

private:
    int         x_pos;
    int         y_pos;
    double      speed;

    RECT        rect;
    RECT        prev_rect;
};// end class

