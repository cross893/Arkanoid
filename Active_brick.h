#pragma once

#include "config.h"

enum E_brick_type
{// Создане перечисления для типа кирпичей
	EBT_none,
	EBT_light_red,
	EBT_cyan
};

//------------------------------------------------------------------------------------------------------------
class C_active_brick
{
public:
	C_active_brick(E_brick_type active_brick_type);

	void F_act(HWND hwnd);
	void F_draw(HDC hdc, RECT& paint_area);
	static void F_setup_colors();

	static const int max_fade_step = 30;

	static HPEN fading_light_red_pens[max_fade_step];
	static HPEN fading_cyan_pens[max_fade_step];
	static HBRUSH fading_light_red_brushes[max_fade_step];
	static HBRUSH fading_cyan_brushes[max_fade_step];

	int fade_step;

	RECT brick_rect;

	E_brick_type brick_type;
};

