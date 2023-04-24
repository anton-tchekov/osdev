/**
 * @file    gfx.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <gfx.h>
#include <syscall.h>

void gfx_rect(i32 x, i32 y, i32 w, i32 h, Color color)
{
	syscall5(SYSCALL_GFX_RECT, x, y, w, h, color);
}

void gfx_rect_border(i32 x, i32 y, i32 w, i32 h, i32 border, Color color)
{
	gfx_rect(x, y, w, border, color);
	gfx_rect(x, y + h - border, w, border, color);

	gfx_rect(x, y + border, border, h - 2 * border, color);
	gfx_rect(x + w - border, y + border, border, h - 2 * border, color);
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
	Rectangle rect = { x, y, w, h };
	syscall4(SYSCALL_GFX_IMAGE_1BIT, (u32)&rect, (u32)data, fg, bg);
}
