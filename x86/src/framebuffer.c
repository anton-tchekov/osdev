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
void framebuffer_rect_outline(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, i32 t, Color color){
	framebuffer_rect(fb, x, y, t, h, color);
	framebuffer_rect(fb, x + w - t, y, t, h, color);

	framebuffer_rect(fb, x + t, y, w - t, t, color);
	framebuffer_rect(fb, x + t, y + h - t, w - t, t, color);

}

void framebuffer_round_rect(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, i32 r, Color color)
{
	framebuffer_rect(fb, x + r, y, w - 2 * r, r, color);
	framebuffer_rect(fb, x, y + r, w, h - 2 * r, color);
	framebuffer_rect(fb, x + r, y + h - r, w - 2 * r, r, color);

	framebuffer_circle_tl(fb, x + r, y + r, r, 0, color, color);
	framebuffer_circle_tr(fb, x + w - r, y + r, r, 0, color, color);
	framebuffer_circle_bl(fb, x + r, y + h - r, r, 0, color, color);
	framebuffer_circle_br(fb, x + w - r, y + h - r, r, 0, color, color);
}
void framebuffer_round_rect_outline(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, i32 r, i32 t, Color color, Color fill)
{
	framebuffer_rect(fb, x + r, y + t, w - 2 * r, r - t, fill);
	framebuffer_rect(fb, x + t, y + r, w - 2 * t, h - 2 * r, fill);
	framebuffer_rect(fb, x + r, y + h - r, w - 2 * r, r - t, fill);

	framebuffer_rect(fb, x + r, y, w - 2*r, t, color);
	framebuffer_rect(fb, x + r, y + h - t, w - 2*r, t, color);
	framebuffer_rect(fb, x, y + r, t, h - 2*r, color);
	framebuffer_rect(fb, x + w - t, y + r, t, h - 2*r, color);

	framebuffer_circle_tl(fb, x + r, y + r, r, r - t, color, fill);
	framebuffer_circle_tr(fb, x + w - r, y + r, r, r - t, color, fill);
	framebuffer_circle_bl(fb, x + r, y + h - r, r, r - t, color, fill);
	framebuffer_circle_br(fb, x + w - r, y + h - r, r, r - t, color, fill);
}

void framebuffer_circle(Framebuffer *fb, i32 sx, i32 sy, i32 r, Color c)
{
	for(i32 y = -r; y <= r; ++y)
	{
		for(i32 x = -r; x <= r; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r * r)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
		}
	}
}
void framebuffer_circle_outline(Framebuffer *fb, i32 sx, i32 sy, i32 r, i32 ir, Color c, Color fill)
{
	i32 r2 = r*r;
	i32 ir2 = ir*ir;

	for(i32 y = -r; y <= r; ++y)
	{
		for(i32 x = -r; x <= r; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r2 && d >= ir2)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
			if(d < ir2) {
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = fill;
			}
		}
	}
}

void framebuffer_circle_tl(Framebuffer *fb, i32 sx, i32 sy, i32 r, i32 ir, Color c, Color fill)
{
	i32 r2 = r*r;
	i32 ir2 = ir*ir;

	for(i32 y = -r; y <= 0; ++y)
	{
		for(i32 x = -r; x <= 0; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r2 && d >= ir2)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
			if(d < ir2) {
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = fill;
			}
		}
	}
}
void framebuffer_circle_tr(Framebuffer *fb, i32 sx, i32 sy, i32 r, i32 ir, Color c, Color fill)
{
	i32 r2 = r*r;
	i32 ir2 = ir*ir;
	
	for(i32 y = -r; y <= 0; ++y)
	{
		for(i32 x = 0; x <= r; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r2 && d >= ir2)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
			if(d < ir2) {
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = fill;
			}
		}
	}
}
void framebuffer_circle_bl(Framebuffer *fb, i32 sx, i32 sy, i32 r, i32 ir, Color c, Color fill)
{
	i32 r2 = r*r;
	i32 ir2 = ir*ir;
	
	for(i32 y = 0; y <= r; ++y)
	{
		for(i32 x = -r; x <= 0; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r2 && d >= ir2)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
			if(d < ir2) {
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = fill;
			}
		}
	}
}
void framebuffer_circle_br(Framebuffer *fb, i32 sx, i32 sy, i32 r, i32 ir, Color c, Color fill)
{
	i32 r2 = r*r;
	i32 ir2 = ir*ir;
	
	for(i32 y = 0; y <= r; ++y)
	{
		for(i32 x = 0; x <= r; ++x)
		{
			i32 d = x * x + y * y;
			if(d <= r2 && d >= ir2)
			{
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = c;
			}
			if(d < ir2) {
				fb->Pixels[fb->Width * (sy + y) + (sx + x)] = fill;
			}
		}
	}
}


