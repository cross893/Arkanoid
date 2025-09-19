#include "Config.h"

// C_config
//------------------------------------------------------------------------------------------------------------
void C_config::F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{// Создание пера и кисти
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}// void C_config::F_create_pen_brush
