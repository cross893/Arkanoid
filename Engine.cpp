#include "Engine.h"

// C_engine
//------------------------------------------------------------------------------------------------------------
C_engine::C_engine()
	: pen_bg(0), pen_white(0), pen_dark_red(0), pen_blue(0), pen_cyan(0), pen_white_fat(0), pen_light_red(0), pen_black(0),
	brush_bg(0), brush_white(0), brush_dark_red(0), brush_blue(0), brush_cyan(0), brush_light_red(0), brush_black(0),
	hwnd{}
{
}// C_engine::C_engine




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

	platform.F_redraw(hwnd);

	SetTimer(hwnd, timer_id, 25, 0);
}// void C_engine::F_init




//------------------------------------------------------------------------------------------------------------
void C_engine::F_draw_frame(HDC hdc, RECT& paint_area)
{// Отрисовка игрового поля
	level.F_draw(hwnd, hdc, paint_area);

	platform.F_draw(hdc, paint_area, pen_bg, brush_bg, pen_dark_red, brush_dark_red, pen_white, brush_white, pen_blue, brush_blue);

	ball.F_draw(hdc, paint_area, pen_bg, brush_bg, pen_white, brush_white);

	border.F_draw(hdc, paint_area, pen_bg, brush_bg, pen_cyan, brush_cyan, pen_white, brush_white);
}// void C_engine::F_draw_frame




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_key_down(E_key_type key_type)
{// Функция нажатия клавиш
	switch (key_type)
	{
	case EKT_left:
		platform.x_pos -= platform.x_step;
		if (platform.x_pos <= C_config::border_x_offset)
			platform.x_pos = C_config::border_x_offset;
		platform.F_redraw(hwnd);
		break;


	case EKT_right:
		platform.x_pos += platform.x_step;
		if (platform.x_pos >= C_config::max_x_pos - platform.width + 1)
			platform.x_pos = C_config::max_x_pos - platform.width + 1;
		platform.F_redraw(hwnd);
		break;


	case EKT_space:
		break;
	}// end switch
	return 0;
}// int C_engine::F_on_key_down




//------------------------------------------------------------------------------------------------------------
int C_engine::F_on_timer()
{
	ball.F_move(hwnd, &level, platform.x_pos, platform.width);

	return 0;
}// int C_engine::F_on_timer
