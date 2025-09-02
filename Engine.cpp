#include "Engine.h"

char level_01[C_level::level_height][C_level::level_width] =
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

// C_ball
//------------------------------------------------------------------------------------------------------------
C_ball::C_ball()
: ball_x_pos(20), ball_y_pos(170), ball_speed(3.0), ball_direction(M_PI - M_PI_4)
{
}




//------------------------------------------------------------------------------------------------------------
void C_ball::F_draw(HDC hdc, RECT& paint_area, C_engine *engine)
{
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &ball_rect))
        return;

    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    Ellipse(hdc, prev_ball_rect.left, prev_ball_rect.top, prev_ball_rect.right, prev_ball_rect.bottom);

    SelectObject(hdc, engine->pen_white);
    SelectObject(hdc, engine->brush_white);
    Ellipse(hdc, ball_rect.left, ball_rect.top, ball_rect.right, ball_rect.bottom);
}// void F_draw_ball




//------------------------------------------------------------------------------------------------------------
void C_ball::F_move(C_engine *engine, C_level *level)
{
    int next_x_pos, next_y_pos;
    int max_x_pos_ball = C_engine::max_x_pos - ball_size;
    int y_pos_platform_ball = C_engine::y_pos_platform - ball_size / 2;

    prev_ball_rect = ball_rect;

    next_x_pos = ball_x_pos + (int)(ball_speed * cos(ball_direction));
    next_y_pos = ball_y_pos - (int)(ball_speed * sin(ball_direction));

    // Отражение шарика от левой рамки
    if (next_x_pos < C_engine::border_x_offset)
    {
        next_x_pos = C_engine::border_x_offset - (next_x_pos - C_engine::border_x_offset); //?
        ball_direction = M_PI - ball_direction;
    }// endif

    // Отражение шарика от верхней рамки
    if (next_y_pos < C_engine::border_y_offset)
    {
        next_y_pos = C_engine::border_y_offset - (next_y_pos - C_engine::border_y_offset);
        ball_direction = -ball_direction;
    }// endif

    // Отражение шарика от правой рамки
    if (next_x_pos > max_x_pos_ball)
    {
        next_x_pos = max_x_pos_ball - (next_x_pos - max_x_pos_ball);
        ball_direction = M_PI - ball_direction;
    }// endif

    // Отражение шарика от нижней рамки
    if (next_y_pos > C_engine::max_y_pos)
    {
        next_y_pos = C_engine::max_y_pos - (next_y_pos - C_engine::max_y_pos);
        ball_direction = M_PI + (M_PI - ball_direction);
    }// endif

    // Отражение шарика от платформы
    if (next_y_pos > y_pos_platform_ball)
    {
        if (next_x_pos >= engine->x_pos_platform && next_x_pos <= engine->x_pos_platform + engine->platform_width)
        {
            next_y_pos = y_pos_platform_ball - (next_y_pos - y_pos_platform_ball);
            ball_direction = M_PI + (M_PI - ball_direction);
        }// endif
    }// endif

    level->F_check_level_brick_hit(next_y_pos, ball_direction);

    ball_x_pos = next_x_pos;
    ball_y_pos = next_y_pos;

    ball_rect.left = ball_x_pos * C_engine::global_scale;
    ball_rect.top = ball_y_pos * C_engine::global_scale;
    ball_rect.right = ball_rect.left + ball_size * C_engine::global_scale;
    ball_rect.bottom = ball_rect.top + ball_size * C_engine::global_scale;

    InvalidateRect(engine->hwnd, &prev_ball_rect, FALSE);
    InvalidateRect(engine->hwnd, &ball_rect, FALSE);
}// void F_move_ball




// C_level
//------------------------------------------------------------------------------------------------------------
void C_level::F_check_level_brick_hit(int& next_y_pos, double &ball_direction)
{
    // Отражение шарика от кирпичей
    int i, j;
    int brick_y_pos = level_y_offset + level_height * cell_height;

    for (i = level_height - 1; i >= 0; i--)
    {
        for (j = 0; j < level_width; j++)
        {
            if (level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_direction = -ball_direction;
            }// endif
        }// end for
        brick_y_pos -= cell_height;
    }// end for
}// int F_check_level_brick_hit




//------------------------------------------------------------------------------------------------------------
void C_level::F_draw_brick(HDC hdc, int x, int y, E_brick_type brick_type)
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
    }// end switch

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * C_engine::global_scale, y * C_engine::global_scale, (x + brick_width) * C_engine::global_scale, (y + brick_height) * C_engine::global_scale, 2 * C_engine::global_scale, 2 * C_engine::global_scale);
}// void F_draw_brick




//------------------------------------------------------------------------------------------------------------
void C_level::F_set_brick_letter_color(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
    }// endif
}// void F_set_brick_letter_color




//------------------------------------------------------------------------------------------------------------
void C_level::F_draw_brick_letter(HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step)
{// Отрисовка падающей буквы
    bool    switch_color;
    double  offset;
    double  rotation_angle;
    int     brick_half_height = brick_height * C_engine::global_scale / 2;
    int     back_part_offset;

    HPEN    front_pen, back_pen;
    HBRUSH  front_brush, back_brush;
    XFORM   xform, old_xform;

    if (!(brick_type == EBT_cyan || brick_type == EBT_light_red))
        return;

    rotation_step = rotation_step % 16;

    if (rotation_step < 8)
        rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    else
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);

    if (rotation_step > 4 && rotation_step <= 12)
        switch_color = brick_type == EBT_cyan;
    else
        switch_color = brick_type == EBT_light_red;

    F_set_brick_letter_color(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (rotation_step == 4 || rotation_step == 12)
    {
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);
        Rectangle(hdc, x, y + brick_half_height - C_engine::global_scale, x + brick_width * C_engine::global_scale, y + brick_half_height);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);
        Rectangle(hdc, x, y + brick_half_height, x + brick_width * C_engine::global_scale, y + brick_half_height + C_engine::global_scale - 1);
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

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)C_engine::global_scale;
        back_part_offset = (int)round(offset);

        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, brick_width * C_engine::global_scale, brick_half_height - back_part_offset);

        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);
        Rectangle(hdc, 0, -brick_half_height, brick_width * C_engine::global_scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_o)
            {
                SelectObject(hdc, pen_white_fat);
                Ellipse(hdc, 0 + 5 * C_engine::global_scale, (-5 * C_engine::global_scale) / 2, 0 + 10 * C_engine::global_scale, (5 * C_engine::global_scale) / 2);
            }// endif
        }// endif

        SetWorldTransform(hdc, &old_xform);
    }// endif
}// void F_draw_brick_letter




//------------------------------------------------------------------------------------------------------------
void C_level::F_draw(HDC hdc, RECT& paint_area)
{// Отрисовка уровня
    int i;
    int j;

    RECT intersection_rect;
    
    if (! IntersectRect(&intersection_rect, &paint_area, &level_rect))
        return;

    for (i = 0; i < level_height; i++)
        for (j = 0; j < level_width; j++)
            F_draw_brick(hdc, level_x_offset + j * cell_width, level_y_offset + i * cell_height, (E_brick_type)level_01[i][j]);
}// void F_draw_level




// C_engine
//------------------------------------------------------------------------------------------------------------
C_engine::C_engine()
: inner_width(21), x_pos_platform(border_x_offset), x_step_platform(global_scale * 2), platform_width(28)
{
}




//------------------------------------------------------------------------------------------------------------
void C_engine::F_init_engine(HWND init_hwnd)
{// Настройка игры при старте
    hwnd = init_hwnd;
    pen_white_fat = CreatePen(PS_SOLID, global_scale, RGB(255, 255, 255)); //?
    F_create_pen_brush(15, 63, 31, pen_bg, brush_bg);
    F_create_pen_brush(255, 255, 255, pen_white, brush_white);
    F_create_pen_brush(0, 0, 0, pen_black, brush_black);
    F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red); //?
    F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan); //?
    F_create_pen_brush(255, 85, 85, level.pen_light_red, level.brush_light_red); //?
    F_create_pen_brush(85, 255, 255, level.pen_cyan, level.brush_cyan); //?
    F_create_pen_brush(151, 0, 0, pen_dark_red, brush_dark_red);
    F_create_pen_brush(0, 128, 192, pen_blue, brush_blue);

    level.level_rect.left = C_level::level_x_offset * global_scale;
    level.level_rect.top = C_level::level_y_offset * global_scale;
    level.level_rect.right = level.level_rect.left + C_level::cell_width * C_level::level_width * global_scale;
    level.level_rect.bottom = level.level_rect.top + C_level::cell_height * C_level::level_height * global_scale;

    F_redraw_platform();

    SetTimer(hwnd, timer_id, 50, 0);
}// void F_init




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_frame(HDC hdc, RECT &paint_area)
{// Отрисовка игрового поля
    RECT intersection_rect;

    level.F_draw(hdc, paint_area);

    if (IntersectRect(&intersection_rect, &paint_area, &platform_rect))
        F_draw_platform(hdc, x_pos_platform, y_pos_platform);

    ball.F_draw(hdc, paint_area, this);

    F_draw_bounds(hdc, paint_area);
}// void F_draw_frame




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_key_down(E_key_type key_type)
{// Функция нажатия клавиш
    switch (key_type)
    {
    case EKT_left:
        x_pos_platform -= x_step_platform;
        if (x_pos_platform <= border_x_offset)
            x_pos_platform = border_x_offset;
        F_redraw_platform();
        break;


    case EKT_right:
        x_pos_platform += x_step_platform;
        if (x_pos_platform >= max_x_pos - platform_width + 1)
            x_pos_platform = max_x_pos - platform_width + 1;
        F_redraw_platform();
        break;


    case EKT_space:
        break;
    }// end switch
    return 0;
}// int F_on_key_down




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_timer()
{
    ball.F_move(this, &level);

    return 0;
}// int F_on_timer




//------------------------------------------------------------------------------------------------------------
void C_engine::F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{// Создание пера и кисти
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}// void F_create_pen_brush




//------------------------------------------------------------------------------------------------------------
void C_engine::F_redraw_platform()
{
    prev_platform_rect =      platform_rect;

    platform_rect.left =      x_pos_platform * global_scale;
    platform_rect.top =       y_pos_platform * global_scale;
    platform_rect.right =     platform_rect.left + platform_width * global_scale;
    platform_rect.bottom =    platform_rect.top + platform_height * global_scale;

    InvalidateRect(hwnd, &prev_platform_rect, FALSE);
    InvalidateRect(hwnd, &platform_rect, FALSE);
}// void F_redraw_platform




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_platform(HDC hdc, int x, int y)
{// Отрисовка платформы

    SelectObject(hdc, pen_bg);
    SelectObject(hdc, brush_bg);
    Rectangle(hdc, prev_platform_rect.left, prev_platform_rect.top, prev_platform_rect.right, prev_platform_rect.bottom);

    // Рисуем боковые шарики
    SelectObject(hdc, pen_dark_red);
    SelectObject(hdc, brush_dark_red);
    Ellipse(hdc, x * global_scale, y * global_scale, (x + circle_size) * global_scale, (y + circle_size) * global_scale);
    Ellipse(hdc, (x + inner_width) * global_scale, y * global_scale, (x + inner_width + circle_size) * global_scale, (y + circle_size) * global_scale);

    // Рисуем блик
    SelectObject(hdc, pen_white);
    Arc(hdc, (x + 1) * global_scale, (y + 1) * global_scale, (x + circle_size - 1) * global_scale, (y + circle_size - 1) * global_scale,
        (x + 1 + 1) * global_scale, (y + 1) * global_scale, (x + 1) * global_scale, (y + 1 + 2) * global_scale);

    // Рисуем средную часть
    SelectObject(hdc, pen_blue);
    SelectObject(hdc, brush_blue);
    RoundRect(hdc, (x + 4) * global_scale, (y + 1) * global_scale, (x + 4 + inner_width - 1) * global_scale, (y + 1 + 5) * global_scale,
        3 * global_scale, 3 * global_scale);
}// void F_draw_platform




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_border(HDC hdc, int x, int y, bool top_border)
{
    // Синяя часть линии
    SelectObject(hdc, pen_cyan);
    SelectObject(hdc, brush_cyan);
    if (top_border)
        Rectangle(hdc, x * global_scale, (y + 1) * global_scale, (x + 4) * global_scale, (y + 4) * global_scale);
    else
        Rectangle(hdc, (x + 1) * global_scale, y * global_scale, (x + 4) * global_scale, (y + 4) * global_scale);

    // Белая часть линии
    SelectObject(hdc, pen_white);
    SelectObject(hdc, brush_white);
    if (top_border)
        Rectangle(hdc, x * global_scale, y * global_scale, (x + 4) * global_scale, (y + 1) * global_scale);
    else
        Rectangle(hdc, x * global_scale, y * global_scale, (x + 1) * global_scale, (y + 4) * global_scale);

    // Черная перфорация
    SelectObject(hdc, pen_bg);
    SelectObject(hdc, brush_bg);
    if (top_border)
        Rectangle(hdc, (x + 2) * global_scale, (y + 2) * global_scale, (x + 3) * global_scale, (y + 3) * global_scale);
    else
        Rectangle(hdc, (x + 2) * global_scale, (y + 1) * global_scale, (x + 3) * global_scale, (y + 2) * global_scale);
}// void F_draw_border




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_bounds(HDC hdc, RECT& paint_area)
{
    int i;

    // Левая граница
    for (i = 0; i < 50; i++)
        F_draw_border(hdc, 2, 1 + i * 4, false);

    // Правая граница
    for (i = 0; i < 50; i++)
        F_draw_border(hdc, 201, 1 + i * 4, false);

    // Верхняя граница
    for (i = 0; i < 50; i++)
        F_draw_border(hdc, 3 + i * 4, 0, true);
}// void F_draw_bounds
