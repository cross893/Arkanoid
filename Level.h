#pragma once

#include "Config.h"

enum E_brick_type
{// Создане перечисления для типа кирпичей
	EBT_none,
	EBT_light_red,
	EBT_cyan
};

enum E_letter_type
{
	ELT_none,
	ELT_o
};

//------------------------------------------------------------------------------------------------------------
class C_level
{
public:
	C_level();

	void F_init();
	void F_check_level_brick_hit(int& next_y_pos, double& direction);
	void F_draw(HDC hdc, RECT& paint_area);

private:
	void F_draw_brick(HDC hdc, int x, int y, E_brick_type brick_type);
	void F_set_brick_letter_color(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void F_draw_brick_letter(HDC hdc, int x, int y, E_brick_type brick_type, E_letter_type letter_type, int rotation_step);

	static const int brick_width = 15;
	static const int brick_height = 7;
	static char level_01[C_config::level_height][C_config::level_width];

	HPEN pen_white_fat, pen_light_red, pen_cyan;
	HBRUSH brush_light_red, brush_cyan;

	RECT rect;
};// end class
