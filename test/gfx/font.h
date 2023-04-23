#ifndef __FONT_H__
#define __FONT_H__

#include <types.h>
#include <gfx.h>

typedef struct
{
	i32 Dummy;
} Font;

void gfx_char(i32 x, i32 y, i32 c, Font *font, Color fg, Color bg);

void gfx_string(i32 x, i32 y, const char *str, Font *font, Color fg, Color bg);

void gfx_string_len(
	i32 x, i32 y, const char *str, i32 len, Font *font, Color fg, Color bg);

void gfx_string_width(const char *str, Font *font);

void gfx_string_width_len(const char *str, i32 len, Font *font);

#endif /* __FONT_H__ */
