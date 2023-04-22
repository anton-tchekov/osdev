#ifndef __GFX_H__
#define __GFX_H__

#include <types.h>

typedef u32 Color;

#define GFX_WIDTH  320
#define GFX_HEIGHT 480

static inline Color gfx_color(i32 r, i32 g, i32 b, i32 a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}

/*
* draws a rectangle from position (x | y), w pixels to left and h pixels down
* color (color) created with 'gfx_color()'
*/
void gfx_rect(int x, int y, int w, int h, Color color);

/*
* draws a rectangle from position (x | y), w pixels to left and h pixels down
* colors is an pointer, for every pixel a new value
* colors (color) created with 'gfx_color()'
*/
void gfx_image(int x, int y, int w, int h, Color* colors);

/*
* draws a String (str) to position (x | y) with a foreground color (fg) and a background color (bg)
* colors created with 'gfx_color()'
*/
void gfx_string(int x, int y, char* str, Color fg, Color bg);

/*
* Convertes RGBA Color to u32 bytes
*/
Color gfx_color(int r, int g, int b, int a);

#endif /* __GFX_H__ */
