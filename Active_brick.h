#pragma once

#include "config.h"

//------------------------------------------------------------------------------------------------------------
class C_active_brick
{
public:
	C_active_brick();

	void F_act(HWND hwnd);
	void F_draw(HDC hdc, RECT& paint_area);

	static const int max_fade_step = 30;

	int fade_step;

	RECT brick_rect;
};

