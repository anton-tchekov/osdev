#include "font.h"
#include "stdlib.h"

static i32 _find_char(const void *key, const void *elem)
{
	return *(const i32 *)key - ((const FontChar *)elem)->Codepoint;
}

static FontChar *_font_char(i32 c, Font *font)
{
	return bsearch(&c, font->Characters, font->NumCharacters,
			sizeof(FontChar), _find_char);
}

i32 font_string_len(
	Framebuffer *fb,
	i32 x, i32 y, const char *s, u32 len, Font *font, Color color)
{
	u32 i;
	i32 c;
	FontChar *fc;
	void (*render_char)(Framebuffer *, i32, i32, i32, i32,
		const u8 *, Color) =
		font->Flags & FONT_FLAG_GRAYSCALE ?
		framebuffer_image_grayscale : framebuffer_image_1bit;

	for(i = 0; i < len; ++i)
	{
		s = utf8_codepoint(s, &c);
		if(!c)
		{
			break;
		}

		if(!(fc = _font_char(c, font)))
		{
			continue;
		}

		if(fc->Size[0] && fc->Size[1])
		{
			render_char(
				fb,
				x + fc->Bearing[0],
				y + (font->Size - fc->Bearing[1]),
				fc->Size[0],
				fc->Size[1],
				font->Bitmap + fc->Offset,
				color);
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
		s = utf8_codepoint(s, &c);
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
