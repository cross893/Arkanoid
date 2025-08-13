#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum E_letter_type
{
    ELT_none,
    ELT_o
};

enum E_brick_type
{// Создане перечисления для типа кирпичей
    EBT_none,
    EBT_light_red,
    EBT_cyan
};

HPEN pen_white_fat, pen_white, pen_light_red, pen_cyan, pen_dark_red, pen_blue;
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
    pen_white_fat = CreatePen(PS_SOLID, G_global_scale, RGB(255, 255, 255));
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
void F_set_brick_letter_color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
    if (is_switch_color)
    {
        front_pen = pen_light_red;
        front_brush = brush_light_red;

        back_pen = pen_cyan;
        back_brush = brush_cyan;
    }
    else
    {
        front_pen = pen_cyan;
        front_brush = brush_cyan;

        back_pen = pen_light_red;
        back_brush = brush_light_red;
    }
}// void F_set_brick_letter_color




//------------------------------------------------------------------------------------------------------------
void F_draw_brick_letter(HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step)
{// Отрисовка падающей буквы
    bool switch_color;
    double offset;
    double rotation_angle;
    int brick_half_height = G_brick_height * G_global_scale / 2;
    int back_part_offset;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xform, old_xform;

    if (!(brick_type == EBT_cyan || brick_type == EBT_light_red))
        return;

    rotation_step = rotation_step % 16;

    if (rotation_step < 8)
        rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    else
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);

    if (rotation_step > 4 && rotation_step <= 12)
    {
        switch_color = brick_type == EBT_cyan;
    }
    else
    {
        switch_color = brick_type == EBT_light_red;
    }
    F_set_brick_letter_color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (rotation_step == 4 || rotation_step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);
        Rectangle(hdc, x, y + brick_half_height - G_global_scale,  x + G_brick_width * G_global_scale, y + brick_half_height);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);
        Rectangle(hdc, x, y + brick_half_height, x + G_brick_width * G_global_scale, y + brick_half_height + G_global_scale - 1);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        xform.eM11 = 1.0f;
        xform.eM12 = 0.0f;
        xform.eM21 = 0.0f;
        xform.eM22 = (float)cos(rotation_angle);
        xform.eDx = (float)x;
        xform.eDy = (float)y + (float)brick_half_height;
        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &xform);

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)G_global_scale;
        back_part_offset = (int)round(offset);

        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, G_brick_width * G_global_scale, brick_half_height - back_part_offset);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);
        Rectangle(hdc, 0, -brick_half_height, G_brick_width * G_global_scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_o)
            {
                SelectObject(hdc, pen_white_fat);
                Ellipse(hdc, 0 + 5 * G_global_scale, (-5 * G_global_scale) / 2, 0 + 10 * G_global_scale, (5 * G_global_scale) / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}// void F_draw_brick_letter




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
    //F_draw_level(hdc);
    //F_draw_platform(hdc, 50, 100);

    int i;

    for (i = 0; i < 16; i++)
    {
        F_draw_brick_letter(hdc, 100, 20 + i * G_cell_width * G_global_scale / 2, EBT_cyan, ELT_o, i);
        F_draw_brick_letter(hdc, 150, 20 + i * G_cell_width * G_global_scale / 2, EBT_light_red, ELT_o, i);
    }
}// void F_draw_frame
