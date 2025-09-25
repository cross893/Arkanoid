#include "Level.h"

char C_level::level_01[C_config::level_height][C_config::level_width] =
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

// C_level
//------------------------------------------------------------------------------------------------------------
C_level::C_level()
	: pen_white_fat(0), pen_light_red(0), pen_cyan(0), brush_light_red(0), brush_cyan(0), rect{}
{
}// C_level::C_level




//------------------------------------------------------------------------------------------------------------
void C_level::F_init()
{
	C_config::F_create_pen_brush(255, 85, 85, pen_light_red, brush_light_red);
	C_config::F_create_pen_brush(85, 255, 255, pen_cyan, brush_cyan);

	rect.left = C_config::level_x_offset * C_config::global_scale;
	rect.top = C_config::level_y_offset * C_config::global_scale;
	rect.right = rect.left + C_config::level_cell_width * C_config::level_width * C_config::global_scale;
	rect.bottom = rect.top + C_config::level_cell_height * C_config::level_height * C_config::global_scale;

}// void C_level::F_init




//------------------------------------------------------------------------------------------------------------
void C_level::F_check_level_brick_hit(int& next_y_pos, double& direction)
{
	// Отражение шарика от кирпичей
	int i, j;
	int brick_y_pos = C_config::level_y_offset + C_config::level_height * C_config::level_cell_height;

	for (i = C_config::level_height - 1; i >= 0; i--)
	{
		for (j = 0; j < C_config::level_width; j++)
		{
			if (level_01[i][j] == 0)
				continue;

			if (next_y_pos < brick_y_pos)
			{
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
				direction = -direction;
			}// endif
		}// end for
		brick_y_pos -= C_config::level_cell_height;
	}// end for
}// void C_level::F_check_level_brick_hit




//------------------------------------------------------------------------------------------------------------
void C_level::F_draw(HWND hwnd, HDC hdc, RECT& paint_area)
{// Отрисовка уровня
	int i;
	int j;

	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &rect))
		return;

	for (i = 0; i < C_config::level_height; i++)
		for (j = 0; j < C_config::level_width; j++)
			F_draw_brick(hdc, C_config::level_x_offset + j * C_config::level_cell_width, C_config::level_y_offset + i * C_config::level_cell_height, (E_brick_type)level_01[i][j]);

	active_brick.F_draw(hdc, paint_area);
}// void C_level::F_draw




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

	RoundRect(hdc, x * C_config::global_scale, y * C_config::global_scale, (x + C_config::brick_width) * C_config::global_scale, (y + C_config::brick_height) * C_config::global_scale, 2 * C_config::global_scale, 2 * C_config::global_scale);
}// void C_level::F_draw_brick




//------------------------------------------------------------------------------------------------------------
void C_level::F_set_brick_letter_color(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
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
}// void C_level::F_set_brick_letter_color




//------------------------------------------------------------------------------------------------------------
void C_level::F_draw_brick_letter(HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step)
{// Отрисовка падающей буквы
	bool    switch_color;
	double  offset;
	double  rotation_angle;
	int     brick_half_height = C_config::brick_height * C_config::global_scale / 2;
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
		Rectangle(hdc, x, y + brick_half_height - C_config::global_scale, x + C_config::brick_width * C_config::global_scale, y + brick_half_height);

		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, x, y + brick_half_height, x + C_config::brick_width * C_config::global_scale, y + brick_half_height + C_config::global_scale - 1);
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

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)C_config::global_scale;
		back_part_offset = (int)round(offset);

		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, C_config::brick_width * C_config::global_scale, brick_half_height - back_part_offset);

		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -brick_half_height, C_config::brick_width * C_config::global_scale, brick_half_height);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_o)
			{
				SelectObject(hdc, pen_white_fat);
				Ellipse(hdc, 0 + 5 * C_config::global_scale, (-5 * C_config::global_scale) / 2, 0 + 10 * C_config::global_scale, (5 * C_config::global_scale) / 2);
			}// endif
		}// endif

		SetWorldTransform(hdc, &old_xform);
	}// endif
}// void C_level::F_draw_brick_letter
