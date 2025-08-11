#include "Engine.h"

enum E_brick_type
{// Создане перечисления для типа кирпичей
    EBT_none,
    EBT_light_red,
    EBT_cyan
};

HPEN pen_white, pen_light_red, pen_cyan, pen_dark_red, pen_blue;
HBRUSH brush_white, brush_light_red, brush_cyan, brush_dark_red, brush_blue;

const int G_global_scale = 3;
const int G_brick_width = 15;
const int G_brick_height = 7;
const int G_cell_width = 16;
const int G_cell_height = 8;
const int G_level_x_offset = 8;
const int G_level_y_offset = 6;
const int G_circle_size = 7;
int G_inner_width = 21;

char level_01[14][12] =
{// Создание первого уровня
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
};// char level_01

//------------------------------------------------------------------------------------------------------------
void F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{// Создание пера и кисти
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}// void F_create_pen_brush




//------------------------------------------------------------------------------------------------------------
void F_init()
{// Присвоение определенного цвета к перу и кисти
    F_create_pen_brush(255, 255, 255, pen_white, brush_white);
    F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red);
    F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);
    F_create_pen_brush(151, 0, 0, pen_dark_red, brush_dark_red);
    F_create_pen_brush(0, 128, 192, pen_blue, brush_blue);
}// void F_init




//------------------------------------------------------------------------------------------------------------
void F_draw_brick(HDC hdc, int x, int y, E_brick_type brick_type)
{// Отрисовка кирпича
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_none:
        return;


    case EBT_light_red:
        pen = pen_light_red;
        brush = brush_light_red;
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

    RoundRect(hdc, x * G_global_scale, y * G_global_scale, (x + G_brick_width) * G_global_scale, (y + G_brick_height) * G_global_scale, 2 * G_global_scale, 2 * G_global_scale);
}// void F_draw_brick




//------------------------------------------------------------------------------------------------------------
void F_draw_level(HDC hdc)
{// Отрисовка уровня
    int i;
    int j;

    for (i = 0; i < 14; i++)
        for (j = 0; j < 12; j++)
            F_draw_brick(hdc, G_level_x_offset + j * G_cell_width, G_level_y_offset + i * G_cell_height, (E_brick_type)level_01[i][j]);
}// void F_draw_level




//------------------------------------------------------------------------------------------------------------
void F_draw_platform(HDC hdc, int x, int y)
{// Отрисовка платформы
    // Рисуем боковые шарики
    SelectObject(hdc, pen_dark_red);
    SelectObject(hdc, brush_dark_red);
    Ellipse(hdc, x * G_global_scale, y * G_global_scale, (x + G_circle_size) * G_global_scale, (y + G_circle_size) * G_global_scale);
    Ellipse(hdc, (x + G_inner_width) * G_global_scale, y * G_global_scale, (x + G_inner_width + G_circle_size) * G_global_scale, (y + G_circle_size) * G_global_scale);

    // Рисуем блик
    SelectObject(hdc, pen_white);
    Arc(hdc, (x + 1) * G_global_scale, (y + 1) * G_global_scale, (x + G_circle_size - 1) * G_global_scale, (y + G_circle_size - 1) * G_global_scale,
        (x + 1 + 1) * G_global_scale, (y + 1) * G_global_scale, (x + 1) * G_global_scale, (y + 1 + 2) * G_global_scale);

    // Рисуем средную часть
    SelectObject(hdc, pen_blue);
    SelectObject(hdc, brush_blue);
    RoundRect(hdc, (x + 4) * G_global_scale, (y + 1) * G_global_scale, (x + 4 + G_inner_width - 1) * G_global_scale, (y + 1 + 5) * G_global_scale,
        3 * G_global_scale, 3 * G_global_scale);
}// void F_draw_platform




//------------------------------------------------------------------------------------------------------------
void F_draw_frame(HDC hdc)
{// Отрисовка игрового поля
    F_draw_level(hdc);
    F_draw_platform(hdc, 50, 100);
}// void F_draw_frame
