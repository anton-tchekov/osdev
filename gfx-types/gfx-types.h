/**
 * @file    gfx.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 *
 * @brief   Graphics types
 */

#ifndef __GFX_TYPES_H__
#define __GFX_TYPES_H__

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

#endif /* __GFX_TYPES_H__ */
