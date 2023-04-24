/**
 * @file    font.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <font.h>
#include <utf8.h>
#include <algo.h>

static i32 _find_char(const void *key, const void *elem)
{
	return *(const i32 *)key - ((const FontChar *)elem)->Character;
}

static FontChar *_font_char(i32 c, Font *font)
{
	return bsearch(&c, font->Characters, font->NumCharacters,
			sizeof(FontChar), _find_char);
}

i32 font_string_len(
	i32 x, i32 y, const char *s, u32 len, Font *font, Color fg, Color bg)
{
	u32 i;
	i32 c;
	FontChar *fc;

	for(i = 0; i < len; ++i)
	{
		s = utf8(s, &c);
		if(!c)
		{
			break;
		}

		if((fc = _font_char(c, font)))
		{
			continue;
		}

		if(fc->Size[0] && fc->Size[1])
		{
			gfx_image_1bit(
				x + fc->Bearing[0],
				y + (font->Size - fc->Bearing[1]),
				fc->Size[0],
				fc->Size[1],
				fc->Image,
				fg, bg);
		}

		x += fc->Advance;
	}

	return x;
}

i32 font_string_width_len(const char *s, u32 len, Font *font)
{
	i32 c, width;
	u32 i;
	FontChar *fc;

	width = 0;
	for(i = 0; i < len; ++i)
	{
		s = utf8(s, &c);
		if(!c)
		{
			break;
		}

		if((fc = _font_char(c, font)))
		{
			width += fc->Advance;
		}
	}

	return width;
}