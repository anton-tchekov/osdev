/**
 * @file    gfx.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __GFX_H__
#define __GFX_H__

#include <types.h>

/** TODO */
typedef u32 Color;

/** The width of the display */
#define GFX_WIDTH  320

/** The height of the display */
#define GFX_HEIGHT 480

/** Rectangle structure consisting of X, Y, Width and Height */
typedef struct
{
	i32 X, Y, W, H;
} Rectangle;

#define GFX_VLINE(x, y, h) gfx_rect(x, y, 1, h)
#define GFX_HLINE(x, y, w) gfx_rect(x, y, w, 1)

/**
 * @brief TODO
 *
 * @param r TODO
 * @param g TODO
 * @param b TODO
 * @param a TODO
 * @return TODO
 */
static inline Color gfx_color(i32 r, i32 g, i32 b, i32 a)
{
	return r | (g << 8) | (b << 16) | (a << 24);
}

/**
 * @brief Draws a rectangle from position (x|y),
 *        w pixels to left and h pixels down
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param color TODO
 */
void gfx_rect(i32 x, i32 y, i32 w, i32 h, Color color);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
* @param border TODO
 * @param color TODO
 */
void gfx_rect_border(i32 x, i32 y, i32 w, i32 h, i32 border, Color color);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_grayscale(i32 x, i32 y, i32 w, i32 h, u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 * @param fg TODO
 * @param bg TODO
 */
void gfx_image_1bit(i32 x, i32 y, i32 w, i32 h, u8 *data, Color fg, Color bg);

#endif /* __GFX_H__ */
