#include "framebuffer.h"
#include "stdlib.h"

void framebuffer_init(Framebuffer *fb, u32 *base, i32 w, i32 h)
{
	fb->Width = w;
	fb->Height = h;
	fb->Pixels = base;
}

void framebuffer_rect(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, Color color)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x + w < fb->Width);
	assert(y + h < fb->Height);

	u32 *start = &fb->Pixels[y * fb->Width + x];
	while (h--)
	{
		memset32(start, color, w);
		start += fb->Width;
	}
}
void framebuffer_circle(Framebuffer *fb, i32 sx, i32 sy, i32 r, Color c)
{
	for(i32 y = -r; y <= r; y++)
	{
		for(i32 x = -r; x <= r; x++)
		{
			if(x * x + y * y <= r * r)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
		}
	}
}

static inline u8 framebuffer_r(u32 color) { return (color)       & 0xFF; }
static inline u8 framebuffer_g(u32 color) { return (color >>  8) & 0xFF; }
static inline u8 framebuffer_b(u32 color) { return (color >> 16) & 0xFF; }
static inline u8 framebuffer_a(u32 color) { return (color >> 24) & 0xFF; }

static inline Color framebuffer_color(u8 r, u8 g, u8 b, u8 a)
{
	return ((Color)a << 24) |
		((Color)b << 16) |
		((Color)g << 8) |
		((Color)r);
}

static u32 _color_merge(u32 color1, u32 color2, u32 ratio)
{
	u8 r1, g1, b1, r2, g2, b2;

	r1 = framebuffer_r(color1);
	g1 = framebuffer_g(color1);
	b1 = framebuffer_b(color1);

	r2 = framebuffer_r(color2);
	g2 = framebuffer_g(color2);
	b2 = framebuffer_b(color2);

	return framebuffer_color(
		(r1 * ratio + r2 * (255 - ratio)) / 255,
		(g1 * ratio + g2 * (255 - ratio)) / 255,
		(b1 * ratio + b2 * (255 - ratio)) / 255, 0xFF);
}

void framebuffer_image_grayscale(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h,
		const u8 *data, Color color)
{
	i32 w0;
	u32 *p, *pixels = &fb->Pixels[y * fb->Width + x];
	while(h--)
	{
		p = pixels;
		w0 = w;
		while(w0--)
		{
			*p = _color_merge(color, *p, *data++);
			++p;
		}

		pixels += fb->Width;
	}
}

void framebuffer_image_1bit(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h,
		const u8 *data, Color color)
{
	u8 byte, stride;
	i32 x0, y0, byte_offset, bit_mask;
	stride = (w + 7) / 8;
	for(y0 = y; y0 < y + h; ++y0)
	{
		bit_mask = 0x80;
		byte_offset = 0;
		for(x0 = x; x0 < x + w; ++x0)
		{
			if(bit_mask == 0x80)
			{
				byte = data[byte_offset++];
				bit_mask = 1;
			}

			if(byte & bit_mask)
			{
				fb->Pixels[y0 * fb->Width + x0] = color;
			}

			bit_mask <<= 1;
		}

		data += stride;
	}
}
