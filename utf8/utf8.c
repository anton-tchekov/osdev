/**
 * @file    utf8.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <utf8.h>

const char *utf8_codepoint(const char *s, i32 *out)
{
	i32 v;
	if((s[0] & 0xF8) == 0xF0)
	{
		v = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
		s += 4;
	}
	else if((s[0] & 0xF0) == 0xE0)
	{
		v = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
		s += 3;
	}
	else if((s[0] & 0xE0) == 0xC0)
	{
		v = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
		s += 2;
	}
	else
	{
		v = s[0];
		s += 1;
	}

	*out = v;
	return s;
}

u32 utf8_length(const char *s)
{
	char c;
	u32 length = 0;

	while((c = *s))
	{
		if((c & 0xF8) == 0xF0)
		{
			s += 4;
		}
		else if((c & 0xf0) == 0xE0)
		{
			s += 3;
		}
		else if((c & 0xE0) == 0xC0)
		{
			s += 2;
		}
		else
		{
			s += 1;
		}

		++length;
	}

	return length;
}
