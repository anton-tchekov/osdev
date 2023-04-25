/**
 * @file    font.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Font rendering with UTF-8 support
 */

#ifndef __FONT_H__
#define __FONT_H__

#include <types.h>
#include <gfx.h>

/** TODO */
typedef struct
{
	/** TODO */
	i32 Character;

	/** TODO */
	i32 Advance;

	/** TODO */
	i32 Size[2];

	/** TODO */
	i32 Bearing[2];

	/** TODO */
	u8 *Image;
} FontChar;

/** TODO */
typedef struct FONT
{
	/** TODO */
	FontChar *Characters;

	/** TODO */
	u32 Size;

	/** TODO */
	u32 NumCharacters;
} Font;

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param s TODO
 * @param len TODO
 * @param font TODO
 * @param fg TODO
 * @param bg TODO
 */
i32 font_string_len(
	i32 x, i32 y, const char *s, u32 len, Font *font, Color fg, Color bg);

/**
 * @brief TODO
 *
 * @param s TODO
 * @param len TODO
 * @param font TODO
 */
i32 font_string_width_len(const char *s, u32 len, Font *font);

/**
 * @brief TODO
 *
 * @param x TODO
 * @param y TODO
 * @param s TODO
 * @param font TODO
 * @param fg TODO
 * @param bg TODO
 */
static inline i32 font_string(
	i32 x, i32 y, const char *s, Font *font, Color fg, Color bg)
{
	return font_string_len(x, y, s, UINT32_MAX, font, fg, bg);
}

/**
 * @brief TODO
 *
 * @param s TODO
 * @param font TODO
 */
static inline i32 font_string_width(const char *s, Font *font)
{
	return font_string_width_len(s, UINT32_MAX, font);
}

#endif /* __FONT_H__ */
