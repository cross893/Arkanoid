#include "Active_brick.h"

// C_active_brick
HPEN C_active_brick::fading_light_red_pens[max_fade_step];
HPEN C_active_brick::fading_cyan_pens[max_fade_step];
HBRUSH C_active_brick::fading_light_red_brushes[max_fade_step];
HBRUSH C_active_brick::fading_cyan_brushes[max_fade_step];
//------------------------------------------------------------------------------------------------------------
C_active_brick::C_active_brick(E_brick_type active_brick_type)
: fade_step(0), brick_type(active_brick_type)
{
}// C_active_brick::C_active_brick




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_act(HWND hwnd)
{
	if (fade_step < max_fade_step - 1)
	{
		++fade_step;

		InvalidateRect(hwnd, &brick_rect, false);
	}
}// C_active_brick::F_act




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_draw(HDC hdc, RECT& paint_area)
{
	HPEN pen = 0;
	HBRUSH brush = 0;

	switch (brick_type)
	{
	case EBT_light_red:
		pen = fading_light_red_pens[fade_step];
		brush = fading_light_red_brushes[fade_step];
		break;


	case EBT_cyan:
		pen = fading_cyan_pens[fade_step];
		brush = fading_cyan_brushes[fade_step];
		break;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	brick_rect.left = (C_config::level_x_offset + C_config::level_cell_width) * C_config::global_scale;
	brick_rect.top = (C_config::level_y_offset + C_config::level_cell_height) * C_config::global_scale;
	brick_rect.right = brick_rect.left + C_config::brick_width * C_config::global_scale;
	brick_rect.bottom = brick_rect.top + C_config::brick_height * C_config::global_scale;

	RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right, brick_rect.bottom, 2 * C_config::global_scale, 2 * C_config::global_scale);
}// C_active_brick::F_draw




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_setup_colors()
{
	int i;
	int max_step = max_fade_step - 1;
	unsigned char r, g, b;

	for (i = 0; i < max_fade_step; i++)
	{
		r = C_config::light_red.R - i * (C_config::light_red.R - C_config::bg_color.R) / max_step;
		g = C_config::light_red.G - i * (C_config::light_red.G - C_config::bg_color.G) / max_step;
		b = C_config::light_red.B - i * (C_config::light_red.B - C_config::bg_color.B) / max_step;

		C_config::F_create_pen_brush(r, g, b, fading_light_red_pens[i], fading_light_red_brushes[i]);


		r = C_config::cyan.R - i * (C_config::cyan.R - C_config::bg_color.R) / max_step;
		g = C_config::cyan.G - i * (C_config::cyan.G - C_config::bg_color.G) / max_step;
		b = C_config::cyan.B - i * (C_config::cyan.B - C_config::bg_color.B) / max_step;

		C_config::F_create_pen_brush(r, g, b, fading_cyan_pens[i], fading_cyan_brushes[i]);
	}
}// C_active_brick::F_setup_colors
