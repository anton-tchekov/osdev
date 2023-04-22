#include <gfx.h>
#include <syscall.h>

Color gfx_color(int r, int g, int b, int a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}

void gfx_rect(int x, int y, int w, int h, Color color)
{
    syscall5(SYSCALL_GFX_RECT, x, y, w, h, color);
}

void gfx_image(int x, int y, int w, int h, Color* colors)
{
    syscall5(SYSCALL_GFX_IMAGE, x, y, w, h, colors);
}

void gfx_string(int x, int y, char* str, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_STRING, x, y, (u32)str, fg, bg);
}