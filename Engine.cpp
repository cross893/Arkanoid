#include "Engine.h"

// C_ball
//------------------------------------------------------------------------------------------------------------
C_ball::C_ball()
: x_pos(20), y_pos(170), speed(2.0), direction(M_PI - M_PI_4), rect{}, prev_rect{}
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

    rect.left = x_pos * C_config::global_scale;
    rect.top = y_pos * C_config::global_scale;
    rect.right = rect.left + size * C_config::global_scale;
    rect.bottom = rect.top + size * C_config::global_scale;

    InvalidateRect(engine->hwnd, &prev_rect, FALSE);
    InvalidateRect(engine->hwnd, &rect, FALSE);
}// void F_move_ball




// C_platform
//------------------------------------------------------------------------------------------------------------
C_platform::C_platform()
    : inner_width(21), x_pos(C_border::x_offset), x_step(C_config::global_scale * 2), width(28), rect{}, prev_rect{}
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

    rect.left = x_pos * C_config::global_scale;
    rect.top = y_pos * C_config::global_scale;
    rect.right = rect.left + width * C_config::global_scale;
    rect.bottom = rect.top + height * C_config::global_scale;

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
    Ellipse(hdc, x * C_config::global_scale, y * C_config::global_scale, (x + C_engine::circle_size) * C_config::global_scale, (y + C_engine::circle_size) * C_config::global_scale);
    Ellipse(hdc, (x + inner_width) * C_config::global_scale, y * C_config::global_scale, (x + inner_width + C_engine::circle_size) * C_config::global_scale, (y + C_engine::circle_size) * C_config::global_scale);

    // Рисуем блик
    SelectObject(hdc, engine->pen_white);
    Arc(hdc, (x + 1) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + C_engine::circle_size - 1) * C_config::global_scale, (y + C_engine::circle_size - 1) * C_config::global_scale,
        (x + 1 + 1) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 1) * C_config::global_scale, (y + 1 + 2) * C_config::global_scale);

    // Рисуем средную часть
    SelectObject(hdc, engine->pen_blue);
    SelectObject(hdc, engine->brush_blue);
    RoundRect(hdc, (x + 4) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 4 + inner_width - 1) * C_config::global_scale, (y + 1 + 5) * C_config::global_scale,
        3 * C_config::global_scale, 3 * C_config::global_scale);
}// void F_draw




// C_engine
//------------------------------------------------------------------------------------------------------------
C_engine::C_engine()
    : pen_bg(0), pen_white(0), pen_dark_red(0), pen_blue(0), pen_cyan(0), pen_white_fat(0), pen_light_red(0), pen_black(0),
    brush_bg(0), brush_white(0), brush_dark_red(0), brush_blue(0), brush_cyan(0), brush_light_red(0), brush_black(0),
    hwnd{}
{
}




//------------------------------------------------------------------------------------------------------------
void C_engine::F_init(HWND init_hwnd)
{// Настройка игры при старте
    hwnd = init_hwnd;
    pen_white_fat = CreatePen(PS_SOLID, C_config::global_scale, RGB(255, 255, 255));
    C_config::F_create_pen_brush(15, 63, 31, pen_bg, brush_bg);
    C_config::F_create_pen_brush(255, 255, 255, pen_white, brush_white);
    C_config::F_create_pen_brush(0, 0, 0, pen_black, brush_black);
    C_config::F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red);
    C_config::F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);
    C_config::F_create_pen_brush(151, 0, 0, pen_dark_red, brush_dark_red);
    C_config::F_create_pen_brush(0, 128, 192, pen_blue, brush_blue);

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

    border.F_draw(hdc, paint_area, pen_bg, brush_bg);
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
