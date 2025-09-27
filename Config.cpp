#include "Config.h"

// C_color
//------------------------------------------------------------------------------------------------------------

C_color::C_color(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b)
{
}

// C_config
//------------------------------------------------------------------------------------------------------------
const C_color C_config::light_red(255, 85, 85);
const C_color C_config::cyan(85, 255, 255);

//------------------------------------------------------------------------------------------------------------
void C_config::F_create_pen_brush(const C_color &color, HPEN& pen, HBRUSH& brush)
{// Создание пера и кисти
	pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
	brush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}// void C_config::F_create_pen_brush

//------------------------------------------------------------------------------------------------------------
void C_config::F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{// Создание пера и кисти
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}// void C_config::F_create_pen_brush
