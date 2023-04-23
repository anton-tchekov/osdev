#ifndef __GFX_H__
#define __GFX_H__

#include <types.h>

typedef u32 Color;

#define GFX_WIDTH  320
#define GFX_HEIGHT 480

/**
 * @brief TODO
 */
static inline Color gfx_color(i32 r, i32 g, i32 b, i32 a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}

/**
 * @brief Draws a rectangle from position (x|y),
 *        w pixels to left and h pixels down
 *
 * TODO
 */
void gfx_rect(i32 x, i32 y, i32 w, i32 h, Color color);

/**
 * @brief TODO
 */
void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 */
void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 */
void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 */
void gfx_image_grayscale(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 */
void gfx_image_1bit(i32 x, i32 y, i32 w, i32 h, u8 *data, Color fg, Color bg);


#endif /* __GFX_H__ */
