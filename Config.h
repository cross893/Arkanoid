#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>

//------------------------------------------------------------------------------------------------------------
class C_color
{
public:
	C_color(unsigned char r, unsigned char g, unsigned char b);

	unsigned char R, G, B;
};// end class

//------------------------------------------------------------------------------------------------------------
class C_config
{
public:
	static void F_create_pen_brush(const C_color &color, HPEN& pen, HBRUSH& brush);
	static void F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

	static const C_color bg_color, light_red, cyan;

	static const int circle_size = 7;
	static const int platform_y_pos = 185;
	static const int border_x_offset = 6;
	static const int border_y_offset = 4;
	static const int global_scale = 3;
	static const int level_width = 12;
	static const int level_height = 14;
	static const int level_x_offset = 8;
	static const int level_y_offset = 6;
	static const int level_cell_width = 16;
	static const int level_cell_height = 8;
	static const int brick_width = 15;
	static const int brick_height = 7;
	static const int ball_size = 4;
	static const int max_x_pos = level_x_offset + level_cell_width * level_width;
	static const int max_y_pos = 199 - ball_size;
	static const int FPS = 40;
};// end class
