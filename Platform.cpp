#include "Platform.h"

// C_platform
//------------------------------------------------------------------------------------------------------------
C_platform::C_platform()
	: inner_width(21), x_pos(C_config::border_x_offset), x_step(C_config::global_scale * 2), width(28), rect{}, prev_rect{}
{
}// C_platform::C_platform




//------------------------------------------------------------------------------------------------------------
void C_platform::F_init()
{

}// void C_platform::F_init




//------------------------------------------------------------------------------------------------------------
void C_platform::F_redraw(HWND hwnd)
{
	prev_rect = rect;

	rect.left = x_pos * C_config::global_scale;
	rect.top = C_config::platform_y_pos * C_config::global_scale;
	rect.right = rect.left + width * C_config::global_scale;
	rect.bottom = rect.top + height * C_config::global_scale;

	InvalidateRect(hwnd, &prev_rect, FALSE);
	InvalidateRect(hwnd, &rect, FALSE);
}// void C_platform::F_redraw




//------------------------------------------------------------------------------------------------------------
void C_platform::F_draw(HDC hdc, RECT& paint_area, HPEN pen_dark_red, HBRUSH brush_dark_red, HPEN pen_white, HBRUSH brush_white, HPEN pen_blue, HBRUSH brush_blue)
{// Отрисовка платформы
	int x = x_pos;
	int y = C_config::platform_y_pos;

	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &rect))
		return;

	SelectObject(hdc, C_config::pen_bg);
	SelectObject(hdc, C_config::brush_bg);
	Rectangle(hdc, prev_rect.left, prev_rect.top, prev_rect.right, prev_rect.bottom);

	// Рисуем боковые шарики
	SelectObject(hdc, pen_dark_red);
	SelectObject(hdc, brush_dark_red);
	Ellipse(hdc, x * C_config::global_scale, y * C_config::global_scale, (x + C_config::circle_size) * C_config::global_scale, (y + C_config::circle_size) * C_config::global_scale);
	Ellipse(hdc, (x + inner_width) * C_config::global_scale, y * C_config::global_scale, (x + inner_width + C_config::circle_size) * C_config::global_scale, (y + C_config::circle_size) * C_config::global_scale);

	// Рисуем блик
	SelectObject(hdc, pen_white);
	Arc(hdc, (x + 1) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + C_config::circle_size - 1) * C_config::global_scale, (y + C_config::circle_size - 1) * C_config::global_scale,
		(x + 1 + 1) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 1) * C_config::global_scale, (y + 1 + 2) * C_config::global_scale);

	// Рисуем средную часть
	SelectObject(hdc, pen_blue);
	SelectObject(hdc, brush_blue);
	RoundRect(hdc, (x + 4) * C_config::global_scale, (y + 1) * C_config::global_scale, (x + 4 + inner_width - 1) * C_config::global_scale, (y + 1 + 5) * C_config::global_scale,
		3 * C_config::global_scale, 3 * C_config::global_scale);
}// void C_platform::F_draw
