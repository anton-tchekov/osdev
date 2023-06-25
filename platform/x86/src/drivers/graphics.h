#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "types.h"
#include "framebuffer.h"

extern u32 _fb_width;
extern u32 _fb_height;

static inline u32 graphics_width(void)
{
	return _fb_width;
}

static inline u32 graphics_height(void)
{
	return _fb_height;
}

static inline u32 graphics_color(u8 r, u8 g, u8 b)
{
	return (u32)r | ((u32)g << 8) | ((u32)b << 16);
}

void graphics_init(u8 *info);

void graphics_rect(u32 x, u32 y, u32 w, u32 h, u32 color);

void graphics_cursor(u32 x, u32 y, u32 w, u32 h, const char *cursor);

void graphics_restore(u32 x, u32 y, u32 w, u32 h);

void graphics_blit_framebuffer(Framebuffer *fb, i32 x, i32 y);

#endif /* __GRAPHICS_H__ */
