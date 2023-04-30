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

/** Font flags bit mask: 1: grayscale, 0: 1 bit per pixel */
#define FONT_FLAG_GRAYSCALE 1

/** Character Data struct */
typedef struct
{
	/** Unicode Codepoin */
	i32 Codepoint;

	/** Pixels to move right after char */
	i32 Advance;

	/** Size of the character bitmap in pixels */
	i32 Size[2];

	/** Offset of the bitmap from origin point in pixels */
	i32 Bearing[2];

	/** Byteoffset in Bitmap */
	i32 Offset;
} FontChar;

/** Font Data struct */
typedef struct FONT
{
	/** Array of  */
	FontChar *Characters;

	/** Font Size (Height) */
	u32 Size;

	/** Total number of characters in font */
	u32 NumCharacters;

	/** Bit 1: Grayscale / 1bpp font */
	u32 Flags;

	/** Character image data */
	u8 *Bitmap;
} Font;

/**
 * @brief Draw an UTF-8 string `s` with a maximum number of chars.
 *        This function will also stop at a '\0' character.
 *
 * @param x X-Coordinate (Top)
 * @param y Y-Coordinate (Left)
 * @param s Character array
 * @param len Maximum number of unicode characters
 * @param font The font to use
 * @param fg Foreground color
 * @param bg Background color
 */
i32 font_string_len(
	i32 x, i32 y, const char *s, u32 len, Font *font, Color fg, Color bg);

/**
 * @brief Calculate the width of a string in pixels
 *
 * @param s The string
 * @param len Maximum number of unicode characters
 * @param font The font
 */
i32 font_string_width_len(const char *s, u32 len, Font *font);

/**
 * @brief Draw an NUL-terminated UTF-8 string `s`
 *
 * @param x X-Coordinate (Top)
 * @param y Y-Coordinate (Left)
 * @param s Character array
 * @param font The font to use
 * @param fg Foreground color
 * @param bg Background color
 */
static inline i32 font_string(
	i32 x, i32 y, const char *s, Font *font, Color fg, Color bg)
{
	return font_string_len(x, y, s, UINT32_MAX, font, fg, bg);
}

/**
 * @brief Calculate the width of a string in pixels
 *
 * @param s The string
 * @param font The font
 */
static inline i32 font_string_width(const char *s, Font *font)
{
	return font_string_width_len(s, UINT32_MAX, font);
}

#endif /* __FONT_H__ */
