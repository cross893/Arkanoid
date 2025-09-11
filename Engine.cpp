#include "Engine.h"

char level_01[C_level::height][C_level::width] =
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
: x_pos(20), y_pos(170), speed(2.0), direction(M_PI - M_PI_4)
{
}




//------------------------------------------------------------------------------------------------------------
void C_ball::F_draw(HDC hdc, RECT& paint_area, C_engine *engine)
{
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &rect))
        return;

    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    Ellipse(hdc, prev_rect.left, prev_rect.top, prev_rect.right, prev_rect.bottom);

    SelectObject(hdc, engine->pen_white);
    SelectObject(hdc, engine->brush_white);
    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}// void F_draw_ball




//------------------------------------------------------------------------------------------------------------
void C_ball::F_move(C_engine *engine, C_level *level, C_platform *platform)
{
    int next_x_pos, next_y_pos;
    int max_x_pos_ball = C_engine::max_x_pos - size;
    int y_pos_platform_ball = C_platform::y_pos - size;

    prev_rect = rect;

    next_x_pos = x_pos + (int)(speed * cos(direction));
    next_y_pos = y_pos - (int)(speed * sin(direction));

    // Отражение шарика от левой рамки
    if (next_x_pos < C_border::x_offset)
    {
        next_x_pos = C_border::x_offset - (next_x_pos - C_border::x_offset); //?
        direction = M_PI - direction;
    }// endif

    // Отражение шарика от верхней рамки
    if (next_y_pos < C_border::y_offset)
    {
        next_y_pos = C_border::y_offset - (next_y_pos - C_border::y_offset);
        direction = -direction;
    }// endif

    // Отражение шарика от правой рамки
    if (next_x_pos > max_x_pos_ball)
    {
        next_x_pos = max_x_pos_ball - (next_x_pos - max_x_pos_ball);
        direction = M_PI - direction;
    }// endif

    // Отражение шарика от нижней рамки
    if (next_y_pos > C_engine::max_y_pos)
    {
        next_y_pos = C_engine::max_y_pos - (next_y_pos - C_engine::max_y_pos);
        direction = M_PI + (M_PI - direction);
    }// endif

    // Отражение шарика от платформы
    if (next_y_pos > y_pos_platform_ball)
    {
        if (next_x_pos >= platform->x_pos && next_x_pos <= platform->x_pos + platform->width)
        {
            next_y_pos = y_pos_platform_ball - (next_y_pos - y_pos_platform_ball);
            direction = M_PI + (M_PI - direction);
        }// endif
    }// endif

    level->F_check_level_brick_hit(next_y_pos, direction);

    x_pos = next_x_pos;
    y_pos = next_y_pos;

    rect.left = x_pos * C_engine::global_scale;
    rect.top = y_pos * C_engine::global_scale;
    rect.right = rect.left + size * C_engine::global_scale;
    rect.bottom = rect.top + size * C_engine::global_scale;

    InvalidateRect(engine->hwnd, &prev_rect, FALSE);
    InvalidateRect(engine->hwnd, &rect, FALSE);
}// void F_move_ball




// C_level
//------------------------------------------------------------------------------------------------------------
void C_level::F_init()
{
    C_engine::F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red);
    C_engine::F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);

    rect.left = C_level::x_offset * C_engine::global_scale;
    rect.top = C_level::y_offset * C_engine::global_scale;
    rect.right = rect.left + C_level::cell_width * C_level::width * C_engine::global_scale;
    rect.bottom = rect.top + C_level::cell_height * C_level::height * C_engine::global_scale;

}// void C_level::F_init




//------------------------------------------------------------------------------------------------------------
void C_level::F_check_level_brick_hit(int& next_y_pos, double &direction)
{
    // Отражение шарика от кирпичей
    int i, j;
    int brick_y_pos = y_offset + height * cell_height;

    for (i = height - 1; i >= 0; i--)
    {
        for (j = 0; j < width; j++)
        {
            if (level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                direction = -direction;
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
    
    if (! IntersectRect(&intersection_rect, &paint_area, &rect))
        return;

    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            F_draw_brick(hdc, x_offset + j * cell_width, y_offset + i * cell_height, (E_brick_type)level_01[i][j]);
}// void F_draw_level




// C_platform
//------------------------------------------------------------------------------------------------------------
C_platform::C_platform()
: inner_width(21), x_pos(C_border::x_offset), x_step(C_engine::global_scale * 2), width(28)
{
}




//------------------------------------------------------------------------------------------------------------
void C_platform::F_init()
{

}// void C_platform::F_init




//------------------------------------------------------------------------------------------------------------
void C_platform::F_redraw(C_engine* engine)
{
    prev_rect = rect;

    rect.left = x_pos * C_engine::global_scale;
    rect.top = y_pos * C_engine::global_scale;
    rect.right = rect.left + width * C_engine::global_scale;
    rect.bottom = rect.top + height * C_engine::global_scale;

    InvalidateRect(engine->hwnd, &prev_rect, FALSE);
    InvalidateRect(engine->hwnd, &rect, FALSE);
}// void F_redraw




//------------------------------------------------------------------------------------------------------------
void C_platform::F_draw(HDC hdc, C_engine *engine, RECT &paint_area)
{// Отрисовка платформы
    int x = x_pos;
    int y = y_pos;

    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &rect))
        return;

    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    Rectangle(hdc, prev_rect.left, prev_rect.top, prev_rect.right, prev_rect.bottom);

    // Рисуем боковые шарики
    SelectObject(hdc, engine->pen_dark_red);
    SelectObject(hdc, engine->brush_dark_red);
    Ellipse(hdc, x * C_engine::global_scale, y * C_engine::global_scale, (x + C_engine::circle_size) * C_engine::global_scale, (y + C_engine::circle_size) * C_engine::global_scale);
    Ellipse(hdc, (x + inner_width) * C_engine::global_scale, y * C_engine::global_scale, (x + inner_width + C_engine::circle_size) * C_engine::global_scale, (y + C_engine::circle_size) * C_engine::global_scale);

    // Рисуем блик
    SelectObject(hdc, engine->pen_white);
    Arc(hdc, (x + 1) * C_engine::global_scale, (y + 1) * C_engine::global_scale, (x + C_engine::circle_size - 1) * C_engine::global_scale, (y + C_engine::circle_size - 1) * C_engine::global_scale,
        (x + 1 + 1) * C_engine::global_scale, (y + 1) * C_engine::global_scale, (x + 1) * C_engine::global_scale, (y + 1 + 2) * C_engine::global_scale);

    // Рисуем средную часть
    SelectObject(hdc, engine->pen_blue);
    SelectObject(hdc, engine->brush_blue);
    RoundRect(hdc, (x + 4) * C_engine::global_scale, (y + 1) * C_engine::global_scale, (x + 4 + inner_width - 1) * C_engine::global_scale, (y + 1 + 5) * C_engine::global_scale,
        3 * C_engine::global_scale, 3 * C_engine::global_scale);
}// void F_draw




// C_border
//------------------------------------------------------------------------------------------------------------
void C_border::F_draw_element(HDC hdc, int x, int y, bool top_border, C_engine *engine)
{
    // Синяя часть линии
    SelectObject(hdc, engine->pen_cyan);
    SelectObject(hdc, engine->brush_cyan);
    if (top_border)
        Rectangle(hdc, x * C_engine::global_scale, (y + 1) * C_engine::global_scale, (x + 4) * C_engine::global_scale, (y + 4) * C_engine::global_scale);
    else
        Rectangle(hdc, (x + 1) * C_engine::global_scale, y * C_engine::global_scale, (x + 4) * C_engine::global_scale, (y + 4) * C_engine::global_scale);

    // Белая часть линии
    SelectObject(hdc, engine->pen_white);
    SelectObject(hdc, engine->brush_white);
    if (top_border)
        Rectangle(hdc, x * C_engine::global_scale, y * C_engine::global_scale, (x + 4) * C_engine::global_scale, (y + 1) * C_engine::global_scale);
    else
        Rectangle(hdc, x * C_engine::global_scale, y * C_engine::global_scale, (x + 1) * C_engine::global_scale, (y + 4) * C_engine::global_scale);

    // Черная перфорация
    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    if (top_border)
        Rectangle(hdc, (x + 2) * C_engine::global_scale, (y + 2) * C_engine::global_scale, (x + 3) * C_engine::global_scale, (y + 3) * C_engine::global_scale);
    else
        Rectangle(hdc, (x + 2) * C_engine::global_scale, (y + 1) * C_engine::global_scale, (x + 3) * C_engine::global_scale, (y + 2) * C_engine::global_scale);
}// void F_draw




//------------------------------------------------------------------------------------------------------------
void C_border::F_draw(HDC hdc, RECT& paint_area, C_engine *engine)
{
    int i;

    // Левая граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 2, 1 + i * 4, false, engine);

    // Правая граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 201, 1 + i * 4, false, engine);

    // Верхняя граница
    for (i = 0; i < 50; i++)
        F_draw_element(hdc, 3 + i * 4, 0, true, engine);
}// void F_draw




// C_engine
//------------------------------------------------------------------------------------------------------------
C_engine::C_engine()
{
}




//------------------------------------------------------------------------------------------------------------
void C_engine::F_init(HWND init_hwnd)
{// Настройка игры при старте
    hwnd = init_hwnd;
    pen_white_fat = CreatePen(PS_SOLID, global_scale, RGB(255, 255, 255));
    F_create_pen_brush(15, 63, 31, pen_bg, brush_bg);
    F_create_pen_brush(255, 255, 255, pen_white, brush_white);
    F_create_pen_brush(0, 0, 0, pen_black, brush_black);
    F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red);
    F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);
    F_create_pen_brush(151, 0, 0, pen_dark_red, brush_dark_red);
    F_create_pen_brush(0, 128, 192, pen_blue, brush_blue);

    level.F_init();
    platform.F_init();
    
    platform.F_redraw(this);

    SetTimer(hwnd, timer_id, 25, 0);
}// void F_init




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_frame(HDC hdc, RECT &paint_area)
{// Отрисовка игрового поля
    level.F_draw(hdc, paint_area);

    platform.F_draw(hdc, this, paint_area);

    ball.F_draw(hdc, paint_area, this);

    border.F_draw(hdc, paint_area, this);
}// void F_draw_frame




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_key_down(E_key_type key_type)
{// Функция нажатия клавиш
    switch (key_type)
    {
    case EKT_left:
        platform.x_pos -= platform.x_step;
        if (platform.x_pos <= C_border::x_offset)
            platform.x_pos = C_border::x_offset;
        platform.F_redraw(this);
        break;


    case EKT_right:
        platform.x_pos += platform.x_step;
        if (platform.x_pos >= max_x_pos - platform.width + 1)
            platform.x_pos = max_x_pos - platform.width + 1;
        platform.F_redraw(this);
        break;


    case EKT_space:
        break;
    }// end switch
    return 0;
}// int F_on_key_down




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_timer()
{
    ball.F_move(this, &level, &platform);

    return 0;
}// int F_on_timer




//------------------------------------------------------------------------------------------------------------
void C_engine::F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{// Создание пера и кисти
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}// void F_create_pen_brush
