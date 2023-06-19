#include "graphics.h"
#include "stdlib.h"

static u8 *_fb;

static u32 _fb_pitch;

static u32 _fb_pixel_bytes;

u32 _fb_width;

u32 _fb_height;

void (*_native_color)(u32, u8 *);

static void _bgr888(u32 color, u8 *nc)
{
	nc[0] = color >> 16;
	nc[1] = color >> 8;
	nc[2] = color;
}

void graphics_init(u8 *info)
{
	u32 fb_addr = *(u32 *)(info + 88);
	u32 fb_bpp = *(((u8 *)info) + 108);
	u32 fb_type = *(((u8 *)info) + 109);

	_fb_pitch = *(u32 *)(info + 96);
	_fb_width = *(u32 *)(info + 100);
	_fb_height = *((u32 *)(((u8 *)info) + 104));

	_fb_pixel_bytes = (fb_bpp + 7) / 8;
	_fb = (u8 *)fb_addr;

	_native_color = _bgr888;
}

static void _graphics_line(u8 *start, u8 *color, u32 width)
{
	while(width--)
	{
		memcpy(start, color, _fb_pixel_bytes);
		start += _fb_pixel_bytes;
	}
}

void graphics_rect(u32 x, u32 y, u32 w, u32 h, u32 color)
{
	u8 nc[4];
	_native_color(color, nc);
	u8 *start = _fb + y * _fb_pitch + x * _fb_pixel_bytes;
	while(h--)
	{
		_graphics_line(start, nc, w);
		start += _fb_pitch;
	}
}
