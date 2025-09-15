#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class C_border
{
public:
    C_border();

    void F_init();
    void F_draw(HDC hdc, RECT& paint_area, HPEN pen_bg, HBRUSH brush_bg, HPEN pen_cyan, HBRUSH brush_cyan, HPEN pen_white, HBRUSH brush_white);

    static const int x_offset = 6;
    static const int y_offset = 4;

private:
    void F_draw_element(HDC hdc, int x, int y, bool top_border, HPEN pen_bg, HBRUSH brush_bg, HPEN pen_cyan, HBRUSH brush_cyan, HPEN pen_white, HBRUSH brush_white);
};// end class
