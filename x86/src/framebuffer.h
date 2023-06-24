#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "types.h"

typedef u32 Color;

typedef struct
{
    i32 Width;
    i32 Height;
    u32 *Pixels;
} Framebuffer;

void framebuffer_init(Framebuffer *fb, u32 *base, i32 w, i32 h);

void framebuffer_rect(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, Color color);

void framebuffer_circle(Framebuffer *fb, i32 sx, i32 sy, i32 r, Color c);

void framebuffer_round_rect(Framebuffer *fb, i32 x, i32 y, i32 w, i32 h, i32 r, Color color);

#endif /* __FRAMEBUFFER_H__ */