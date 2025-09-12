#pragma once

#include <windows.h>

//------------------------------------------------------------------------------------------------------------
class C_config
{
public:
    static void F_create_pen_brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

    static const int global_scale = 3;
};
