#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class C_platform
{
public:
    C_platform();

    void F_init();
    void F_redraw(HWND hwnd);
    void F_draw(HDC hdc, RECT& paint_area, HPEN pen_bg, HBRUSH brush_bg, HPEN pen_dark_red, HBRUSH brush_dark_red, HPEN pen_white, HBRUSH brush_white, HPEN pen_blue, HBRUSH brush_blue);

    int x_pos;
    int x_step;
    int width;

private:
    static const int height = 7;

    int inner_width;

    RECT rect;
    RECT prev_rect;
};// end class

