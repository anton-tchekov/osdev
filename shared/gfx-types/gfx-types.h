/**
 * @file    gfx-types.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 * @brief   Graphics types
 */

#ifndef __GFX_TYPES_H__
#define __GFX_TYPES_H__

/** RGBA Color Type */
typedef u32 Color;

/** The width of the display */
#define GFX_WIDTH  320

/** The height of the display */
#define GFX_HEIGHT 480

/** Rectangle structure consisting of X, Y, Width and Height */
typedef struct
{
	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Width */
	i32 W;

	/** Height */
	i32 H;
} Rectangle;

/**
 * @brief Extract red channel from ABGR color
 *
 * @param color ABGR Color
 * @return Red value
 */
static inline u8 _abgr_r(u32 color) { return (color >> 24) & 0xFF; }

/**
 * @brief Extract green channel from ABGR color
 *
 * @param color ABGR Color
 * @return Green value
 */
static inline u8 _abgr_g(u32 color) { return (color >> 16) & 0xFF; }

/**
 * @brief Extract blue channel from ABGR color
 *
 * @param color ABGR Color
 * @return Blue value
 */
static inline u8 _abgr_b(u32 color) { return (color >>  8) & 0xFF; }

#endif /* __GFX_TYPES_H__ */
