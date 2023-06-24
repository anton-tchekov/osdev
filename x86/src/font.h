#ifndef __FONT_H__
#define __FONT_H__

#include "types.h"
#include "framebuffer.h"

/** Font flags bit mask: 1: grayscale, 0: 1 bit per pixel */
#define FONT_FLAG_GRAYSCALE 1

/** Character Data struct */
typedef struct
{
	/** Unicode codepoint */
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
typedef struct
{
	/** Array of characters */
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

i32 font_string_len(
	Framebuffer *fb,
	i32 x, i32 y, const char *s, u32 len, Font *font, Color color);

i32 font_string_width_len(const char *s, u32 len, Font *font);

static inline i32 font_string(
	Framebuffer *fb,
	i32 x, i32 y, const char *s, Font *font, Color color)
{
	return font_string_len(fb, x, y, s, UINT32_MAX, font, color);
}

static inline i32 font_string_width(const char *s, Font *font)
{
	return font_string_width_len(s, UINT32_MAX, font);
}

#endif /* __FONT_H__ */
