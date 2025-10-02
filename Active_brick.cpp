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

	for (i = 0; i < max_fade_step; i++)
	{
		F_get_fading_color(C_config::light_red, i, fading_light_red_pens[i], fading_light_red_brushes[i]);
		F_get_fading_color(C_config::cyan, i, fading_cyan_pens[i], fading_cyan_brushes[i]);
	}
}// C_active_brick::F_setup_colors




//------------------------------------------------------------------------------------------------------------
unsigned char C_active_brick::F_get_fading_channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - step * (color - bg_color) / max_fade_step - 1;
}// C_active_brick::F_get_fading_channel




//------------------------------------------------------------------------------------------------------------
void C_active_brick::F_get_fading_color(const C_color &color, int step, HPEN &pen, HBRUSH &brush)
{
	unsigned char r, g, b;

	r = F_get_fading_channel(color.R, C_config::bg_color.R, step);
	g = F_get_fading_channel(color.G, C_config::bg_color.G, step);
	b = F_get_fading_channel(color.B, C_config::bg_color.B, step);

	C_config::F_create_pen_brush(r, g, b, pen, brush);
}// C_active_brick::F_get_fading_color
