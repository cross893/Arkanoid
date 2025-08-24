#pragma once

#include <windows.h>

enum E_key_type
{
    EKT_left,
    EKT_right,
    EKT_space
};

const int G_timer_id = WM_USER + 1;

void F_init_engine(HWND hwnd);
void F_draw_frame(HDC hdc, RECT& paint_area);
int F_on_key_down(E_key_type key_type);
int F_on_timer();
