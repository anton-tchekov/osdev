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
#include <gfx-types.h>

/** RGBA Color Type */
typedef u32 Color;

/** The width of the display */
#define GFX_WIDTH  320

/** The height of the display */
#define GFX_HEIGHT 480

/**
 * @brief Create a color from R, G, B, A values
 *
 * @param r Red channel (0-255)
 * @param g Green channel (0-255)
 * @param b Blue channel (0-255)
 * @param a Alpha channel (0-255)
 * @return Color value
 */
static inline Color gfx_color(i32 r, i32 g, i32 b, i32 a)
{
	return a | (b << 8) | (g << 16) | (r << 24);
}

/**
 * @brief Draws a rectangle from position (x|y),
 *        w pixels to left and h pixels down
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param color Color from `gfx_color` or `color.h`
 */
void gfx_rect(i32 x, i32 y, i32 w, i32 h, Color color);

/**
 * @brief Draws a 1 pixel wide vertical line
 *
 * @param x Starting X-Coordinate (Left)
 * @param y Starting Y-Coordinate (Top)
 * @param h Length of the line
 * @param color Color from `gfx_color` or `color.h`
 */
static inline void gfx_vline(i32 x, i32 y, i32 h, Color color)
{
	gfx_rect(x, y, 1, h, color);
}

/**
 * @brief Draws a 1 pixel tall horizontal line
 *
 * @param x Starting X-Coordinate (Left)
 * @param y Starting Y-Coordinate (Top)
 * @param w Length of the line
 * @param color Color from `gfx_color` or `color.h`
 */
static inline void gfx_hline(i32 x, i32 y, i32 w, Color color)
{
	gfx_rect(x, y, w, 1, color);
}

/**
 * @brief Draws a rectangular frame
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param border Inset border width in pixels
 * @param color Color of the border
 */
void gfx_rect_border(i32 x, i32 y, i32 w, i32 h, i32 border, Color color);

/**
 * @brief Draws an image in RGBA format
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (32 bits per pixel)
 */
void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief Draws an image in RGB format
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (24 bits per pixel)
 */
void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief Draws an image in RGB565 format
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (16 bit per pixel)
 */
void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief Draws a grayscale image.
 *        The color of each pixel is a mix of the foreground and background
 *        colors where the ratio depends on the grayscale value.
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (8 bit per pixel)
 * @param fg Foreground color
 * @param bg Background color
 */
void gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, const u8 *data, Color fg, Color bg);

/**
 * @brief Draws a 1-bit-per-pixel image.
 *        Every image row starts at a new byte.
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (1 bit per pixel)
 * @param fg Foreground color (bit 1)
 * @param bg Background color (bit 0)
 */
void gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, const u8 *data, Color fg, Color bg);

#endif /* __GFX_H__ */
