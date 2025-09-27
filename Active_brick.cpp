#include "Active_brick.h"

// C_active_brick
//------------------------------------------------------------------------------------------------------------
C_active_brick::C_active_brick()
	: fade_step(0)
{
}// C_active_brick::C_active_brick




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_act(HWND hwnd)
{
	if (fade_step < max_fade_step)
	{
		++fade_step;

		InvalidateRect(hwnd, &brick_rect, false);
	}
}// C_active_brick::F_act




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_draw(HDC hdc, RECT& paint_area)
{
	HPEN pen;
	HBRUSH brush;

	C_config::F_create_pen_brush(C_config::cyan.R - fade_step * (C_config::cyan.R / max_fade_step), C_config::cyan.G - fade_step * (C_config::cyan.G / max_fade_step),
		C_config::cyan.B - fade_step * (C_config::cyan.B / max_fade_step), pen, brush);

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	brick_rect.left = (C_config::level_x_offset + C_config::level_cell_width) * C_config::global_scale;
	brick_rect.top = (C_config::level_y_offset + C_config::level_cell_height) * C_config::global_scale;
	brick_rect.right = brick_rect.left + C_config::brick_width * C_config::global_scale;
	brick_rect.bottom = brick_rect.top + C_config::brick_height * C_config::global_scale;

	RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right, brick_rect.bottom, 2 * C_config::global_scale, 2 * C_config::global_scale);
}// C_active_brick::F_draw
