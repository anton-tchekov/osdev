#include "utf8.h"

i32 codepoint_utf8(i32 codepoint, char *out)
{
	if(codepoint <= 0x7F)
	{
		/* Plain ASCII */
		out[0] = codepoint;
		return 1;
	}
	else if(codepoint <= 0x07FF)
	{
		/* 2-byte unicode */
		out[0] = ((codepoint >> 6) & 0x1F) | 0xC0;
		out[1] = (codepoint        & 0x3F) | 0x80;
		return 2;
	}
	else if(codepoint <= 0xFFFF)
	{
		/* 3-byte unicode */
		out[0] = ((codepoint >> 12) & 0x0F) | 0xE0;
		out[1] = ((codepoint >>  6) & 0x3F) | 0x80;
		out[2] = (codepoint         & 0x3F) | 0x80;
		return 3;
	}
	else if(codepoint <= 0x10FFFF)
	{
		/* 4-byte unicode */
		out[0] = ((codepoint >> 18) & 0x07) | 0xF0;
		out[1] = ((codepoint >> 12) & 0x3F) | 0x80;
		out[2] = ((codepoint >>  6) & 0x3F) | 0x80;
		out[3] = (codepoint         & 0x3F) | 0x80;
		return 4;
	}
	else
	{
		/* error */
		return 0;
	}
}

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
