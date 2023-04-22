#include <gfx.h>
#include <syscall.h>

void gfx_rect(i32 x, i32 y, i32 w, i32 h, Color color)
{
    syscall5(SYSCALL_GFX_RECT, x, y, w, h, color);
}

void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u8 *data)
{
    syscall5(SYSCALL_GFX_IMAGE_RGBA, x, y, w, h, (u32)data);
}

void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u8 *data)
{
    syscall5(SYSCALL_GFX_IMAGE_RGB, x, y, w, h, (u32)data);
}

void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u8 *data)
{
    syscall5(SYSCALL_GFX_IMAGE_RGB565, x, y, w, h, (u32)data);
}

void gfx_image_grayscale(i32 x, i32 y, i32 w, i32 h, u8 *data)
{
    syscall5(SYSCALL_GFX_IMAGE_GRAYSCALE, x, y, w, h, (u32)data);
}

void gfx_image_1bit(i32 x, i32 y, i32 w, i32 h, u8 *data, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_IMAGE_1BIT, x, y, w, h, (u32)data);
}

void gfx_char(i32 x, i32 y, i32 c, Color fg, Color bg)
{

}

void gfx_string(i32 x, i32 y, const char *str, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_STRING, x, y, (u32)str, fg, bg);
}

void gfx_string_len(i32 x, i32 y, const char *str, i32 len, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_STRING, x, y, (u32)str, fg, bg);
}

void gfx_string_width(i32 x, i32 y, const char *str, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_STRING, x, y, (u32)str, fg, bg);
}

void gfx_string_width_len(i32 x, i32 y, const char *str, i32 len, Color fg, Color bg)
{
    syscall5(SYSCALL_GFX_STRING, x, y, (u32)str, fg, bg);
}

