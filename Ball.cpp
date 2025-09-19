#include "Ball.h"

// C_ball
//------------------------------------------------------------------------------------------------------------
C_ball::C_ball()
	: x_pos(20), y_pos(170), speed(2.0), direction(M_PI - M_PI_4), rect{}, prev_rect{}
{
}// C_ball::C_ball




//------------------------------------------------------------------------------------------------------------
void C_ball::F_draw(HDC hdc, RECT& paint_area, HPEN pen_bg, HBRUSH brush_bg, HPEN pen_white, HBRUSH brush_white)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &rect))
		return;

	SelectObject(hdc, pen_bg);
	SelectObject(hdc, brush_bg);
	Ellipse(hdc, prev_rect.left, prev_rect.top, prev_rect.right, prev_rect.bottom);

	SelectObject(hdc, pen_white);
	SelectObject(hdc, brush_white);
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}// void C_ball::F_draw




//------------------------------------------------------------------------------------------------------------
void C_ball::F_move(HWND hwnd, C_level* level, int platform_x_pos, int platform_width)
{
	int next_x_pos, next_y_pos;
	int max_x_pos_ball = C_config::max_x_pos - C_config::ball_size;
	int y_pos_platform_ball = C_config::platform_y_pos - C_config::ball_size;

	prev_rect = rect;

	next_x_pos = x_pos + (int)(speed * cos(direction));
	next_y_pos = y_pos - (int)(speed * sin(direction));

	// Отражение шарика от левой рамки
	if (next_x_pos < C_config::border_x_offset)
	{
		next_x_pos = C_config::border_x_offset - (next_x_pos - C_config::border_x_offset); //?
		direction = M_PI - direction;
	}// endif

	// Отражение шарика от верхней рамки
	if (next_y_pos < C_config::border_y_offset)
	{
		next_y_pos = C_config::border_y_offset - (next_y_pos - C_config::border_y_offset);
		direction = -direction;
	}// endif

	// Отражение шарика от правой рамки
	if (next_x_pos > max_x_pos_ball)
	{
		next_x_pos = max_x_pos_ball - (next_x_pos - max_x_pos_ball);
		direction = M_PI - direction;
	}// endif

	// Отражение шарика от нижней рамки
	if (next_y_pos > C_config::max_y_pos)
	{
		next_y_pos = C_config::max_y_pos - (next_y_pos - C_config::max_y_pos);
		direction = M_PI + (M_PI - direction);
	}// endif

	// Отражение шарика от платформы
	if (next_y_pos > y_pos_platform_ball)
	{
		if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
		{
			next_y_pos = y_pos_platform_ball - (next_y_pos - y_pos_platform_ball);
			direction = M_PI + (M_PI - direction);
		}// endif
	}// endif

	level->F_check_level_brick_hit(next_y_pos, direction);

	x_pos = next_x_pos;
	y_pos = next_y_pos;

	rect.left = x_pos * C_config::global_scale;
	rect.top = y_pos * C_config::global_scale;
	rect.right = rect.left + C_config::ball_size * C_config::global_scale;
	rect.bottom = rect.top + C_config::ball_size * C_config::global_scale;

	InvalidateRect(hwnd, &prev_rect, FALSE);
	InvalidateRect(hwnd, &rect, FALSE);
}// void C_ball::F_move
