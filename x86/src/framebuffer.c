#include "framebuffer.h"

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

	i32 originX = sx - r;
	i32 originY = sy - r;

	for (i32 y = -r; y <= r; y++)
	{
		for (i32 x = -r; x <= r; x++)
		{
			if (x * x + y * y <= r * r)
			{
				fb->Pixels[fb->Width * (originY + y) + (originX + x)] = c;
			}
		}
	}
}