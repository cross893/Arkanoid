#include "Engine.h"

enum Ebrick_type
{
    EBT_none,
    EBT_red,
    EBT_cyan
};

HPEN pen_red, pen_cyan;
HBRUSH brush_red, brush_cyan;

const int Gglobal_scale = 3;
const int Gbrick_width = 15;
const int Gbrick_height = 7;
const int Gcell_width = 16;
const int Gcell_height = 8;
const int Glevel_x_offset = 8;
const int Glevel_y_offset = 6;

char level_01[14][12] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//------------------------------------------------------------------------------------------------------------
void Finit()
{
    pen_red = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
    brush_red = CreateSolidBrush(RGB(255, 85, 85));
    pen_cyan = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
    brush_cyan = CreateSolidBrush(RGB(85, 255, 255));
}




//------------------------------------------------------------------------------------------------------------
void Fdraw_brick(HDC hdc, int x, int y, Ebrick_type brick_type)
{
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_none:
        return;


    case EBT_red:
        pen = pen_red;
        brush = brush_red;
        break;


    case EBT_cyan:
        pen = pen_cyan;
        brush = brush_cyan;
        break;


    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * Gglobal_scale, y * Gglobal_scale, (x + Gbrick_width) * Gglobal_scale, (y + Gbrick_height) * Gglobal_scale, 2 * Gglobal_scale, 2 * Gglobal_scale);
}




//------------------------------------------------------------------------------------------------------------
void Fdraw_level(HDC hdc)
{
    int i;
    int j;

    for (i = 0; i < 14; i++)
        for (j = 0; j < 12; j++)
            Fdraw_brick(hdc, Glevel_x_offset + j * Gcell_width, Glevel_y_offset + i * Gcell_height, (Ebrick_type)level_01[i][j]);
}




//------------------------------------------------------------------------------------------------------------
void Fdraw_frame(HDC hdc)
{
    Fdraw_level(hdc);
}
