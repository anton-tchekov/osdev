#include "graphics.h"
#include "stdlib.h"
#include "font.c"

static u8 *_fb;

static u32 _fb_pitch;

static u32 _fb_pixel_bytes;

u32 _fb_width;

u32 _fb_height;

static u8 _color_white[4], _color_black[4];

/* Double buffer address */
static u8 *_fb_double = (u8 *)0x1000000;

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

	_native_color(graphics_color(0xFF, 0xFF, 0xFF), _color_white);
	_native_color(graphics_color(0x00, 0x00, 0x00), _color_black);

	graphics_rect(0, 0, graphics_width(), graphics_height(), graphics_color(0x00, 0x00, 0x00));
}

static inline u32 _framebuffer_offset(u32 x, u32 y)
{
	return y * _fb_pitch + x * _fb_pixel_bytes;
}

static void _graphics_line(u8 *start, u8 *dbl, u8 *color, u32 width)
{
	while(width--)
	{
		memcpy(start, color, _fb_pixel_bytes);
		memcpy(dbl, color, _fb_pixel_bytes);
		start += _fb_pixel_bytes;
		dbl += _fb_pixel_bytes;
	}
}

void graphics_rect(u32 x, u32 y, u32 w, u32 h, u32 color)
{
	u32 offset;
	u8 nc[4], *start, *dbl;

	_native_color(color, nc);
	offset = _framebuffer_offset(x, y);
	dbl = _fb_double + offset;
	start = _fb + offset;
	while(h--)
	{
		_graphics_line(start, dbl, nc, w);
		start += _fb_pitch;
		dbl += _fb_pitch;
	}
}

void graphics_cursor(u32 x, u32 y, u32 w, u32 h, const char *cursor)
{
	/* Cursor is not drawn on the double buffer so it can be used
		to restore the pixels behind the cursor */
	u32 width;
	u8 *start, *line;

	start = _fb + _framebuffer_offset(x, y);
	while(h--)
	{
		width = w;
		line = start;
		while(width--)
		{
			if(*cursor == 'X')
			{
				memcpy(line, _color_black, _fb_pixel_bytes);
			}
			else if(*cursor == '_')
			{
				memcpy(line, _color_white, _fb_pixel_bytes);
			}

			line += _fb_pixel_bytes;
			++cursor;
		}

		start += _fb_pitch;
	}
}

void graphics_restore(u32 x, u32 y, u32 w, u32 h)
{
	/* Restore pixels that are behind the cursor */
	u32 w_copy, offset;
	u8 *start_src, *start_dst, *line_src, *line_dst;

	offset = _framebuffer_offset(x, y);
	start_src = _fb_double + offset;
	start_dst = _fb + offset;
	while(h--)
	{
		w_copy = w;
		line_dst = start_dst;
		line_src = start_src;
		while(w_copy--)
		{
			memcpy(line_dst, line_src, _fb_pixel_bytes);
			line_dst += _fb_pixel_bytes;
			line_src += _fb_pixel_bytes;
		}

		start_dst += _fb_pitch;
		start_src += _fb_pitch;
	}
}

u8 graphics_char(u16 x, u16 y, char c)
{
	u8 w0, h0;
	const u8 *start;

	start = _font5x7 + (c - 32) * FONT_WIDTH;
	for(h0 = 0; h0 < FONT_HEIGHT; ++h0)
	{
		for(w0 = 0; w0 < FONT_WIDTH; ++w0)
		{
			if((start[w0] >> h0) & 1)
			{
				graphics_rect(x + w0, y + h0, 1, 1, graphics_color(0xFF, 0xFF, 0xFF));
			}
		}
	}

	return FONT_WIDTH + 1;
}

u16 graphics_string(u16 x, u16 y, const char *s)
{
	u16 i;
	char c;

	i = 0;
	while((c = *s++))
	{
		i += graphics_char(x + i, y, c);
	}

	return i;
}

void graphics_blit_framebuffer(Framebuffer *fb, i32 x, i32 y)
{
	u8 *start, *dbl, *s, *d, nc[4];
	u32 w, h, *pixels;

	u32 offset = _framebuffer_offset(x, y);
	dbl = _fb_double + offset;
	start = _fb + offset;
	pixels = fb->Pixels;
	h = fb->Height;
	while(h--)
	{
		w = fb->Width;
		d = dbl;
		s = start;
		while(w--)
		{
			_native_color(*pixels, nc);
			memcpy(s, nc, _fb_pixel_bytes);
			memcpy(d, nc, _fb_pixel_bytes);
			s += _fb_pixel_bytes;
			d += _fb_pixel_bytes;
			++pixels;
		}

		start += _fb_pitch;
		dbl += _fb_pitch;
	}
}

