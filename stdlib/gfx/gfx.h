/**
 * @file    gfx.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Basic Graphics Functions
 */

#ifndef __GFX_H__
#define __GFX_H__

#include <types.h>
#include <colors.h>

/** RGBA Color Type */
typedef u32 Color;

/** The width of the display */
#define GFX_WIDTH  320

/** The height of the display */
#define GFX_HEIGHT 480

/** Rectangle structure consisting of X, Y, Width and Height */
typedef struct
{
	/** TODO */
	i32 X;

	/** TODO */
	i32 Y;

	/** TODO */
	i32 W;

	/** TODO */
	i32 H;
} Rectangle;

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
	return a | (b << 8) | (g << 16) | (r << 24);
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
 * @param h TODO
 */
static inline void gfx_vline(i32 x, i32 y, i32 h, Color color)
{
	gfx_rect(x, y, 1, h, color);
}

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param h TODO
 */
static inline void gfx_hline(i32 x, i32 y, i32 w, Color color)
{
	gfx_rect(x, y, w, 1, color);
}

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
void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param w TODO
 * @param h TODO
 * @param data TODO
 */
void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, const u8 *data);

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
void gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, const u8 *data, Color fg, Color bg);

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
void gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, const u8 *data, Color fg, Color bg);

#endif /* __GFX_H__ */
