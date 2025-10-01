#pragma once

#include <windows.h>

#include "Border.h"
#include "Ball.h"
#include "Platform.h"

enum E_key_type
{
	EKT_left,
	EKT_right,
	EKT_space
};

const int timer_id = WM_USER + 1;

//------------------------------------------------------------------------------------------------------------
class C_engine
{
public:
	C_engine();

	void F_init(HWND hwnd);
	void F_draw_frame(HDC hdc, RECT& paint_area);
	int F_on_key_down(E_key_type key_type);
	int F_on_timer();

private:
	HWND hwnd;
	HPEN pen_white_fat, pen_light_red, pen_black, pen_white, pen_dark_red, pen_blue, pen_cyan;
	HBRUSH brush_light_red, brush_black, brush_white, brush_dark_red, brush_blue, brush_cyan;

	C_ball ball;
	C_level level;
	C_platform platform;
	C_border border;
};// end class
