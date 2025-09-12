#include "Border.h"

// C_border
//------------------------------------------------------------------------------------------------------------
C_border::C_border()
    : pen_cyan(0), pen_white(0), brush_cyan(0), brush_white(0)
{
}




//------------------------------------------------------------------------------------------------------------
void C_border::F_init()
{
    C_config::F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);
    C_config::F_create_pen_brush(255, 255, 255, pen_white, brush_white);
}// void C_border::F_init




//------------------------------------------------------------------------------------------------------------
void C_border::F_draw_element(HDC hdc, int x, int y, bool top_border, HPEN pen_bg, HBRUSH brush_bg)
{
    // Синяя часть линии
    SelectObject(hdc, pen_cyan);
    SelectObject(hdc, brush_cyan);
    if (top_border)
        Rectangle(hdc, x * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 4) * C_config::global_scale, (y + 4) * C_config::global_scale);
    else
        Rectangle(hdc, (x + 1) * C_config::global_scale, y * C_config::global_scale, (x + 4) * C_config::global_scale, (y + 4) * C_config::global_scale);

    // Белая часть линии
    SelectObject(hdc, pen_white);
    SelectObject(hdc, brush_white);
    if (top_border)
        Rectangle(hdc, x * C_config::global_scale, y * C_config::global_scale, (x + 4) * C_config::global_scale, (y + 1) * C_config::global_scale);
    else
        Rectangle(hdc, x * C_config::global_scale, y * C_config::global_scale, (x + 1) * C_config::global_scale, (y + 4) * C_config::global_scale);

    // Черная перфорация
    SelectObject(hdc, pen_bg);
    SelectObject(hdc, brush_bg);
    if (top_border)
        Rectangle(hdc, (x + 2) * C_config::global_scale, (y + 2) * C_config::global_scale, (x + 3) * C_config::global_scale, (y + 3) * C_config::global_scale);
    else
        Rectangle(hdc, (x + 2) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 3) * C_config::global_scale, (y + 2) * C_config::global_scale);
}// void F_draw




//------------------------------------------------------------------------------------------------------------
void C_border::F_draw(HDC hdc, RECT& paint_area, HPEN pen_bg, HBRUSH brush_bg)
{
    int i;

    // Левая граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 2, 1 + i * 4, false, pen_bg, brush_bg);

    // Правая граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 201, 1 + i * 4, false, pen_bg, brush_bg);

    // Верхняя граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 3 + i * 4, 0, true, pen_bg, brush_bg);
}// void F_draw
