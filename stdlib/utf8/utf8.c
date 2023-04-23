/**
 * @file    utf8.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <utf8.h>

const char *utf8(const char *s, i32 *out)
{
	if((s[0] & 0xF8) == 0xF0)
	{
		*out = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
		s += 4;
	}
	else if((s[0] & 0xF0) == 0xE0)
	{
		*out = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
		s += 3;
	}
	else if((s[0] & 0xE0) == 0xC0)
	{
		*out = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
		s += 2;
	}
	else
	{
		*out = s[0];
		s += 1;
	}

	return s;
}
