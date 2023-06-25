#include "memcpy.h"

void *memcpy(void *dest, const void *src, u32 count)
{
	u8 *d, *s, *end;

	s = (u8 *)src;
	d = (u8 *)dest;
	end = d + count;

	if(d != s)
	{
		while(d != end)
		{
			*d++ = *s++;
		}
	}

	return dest;
}

void *memmove(void *dest, const void *src, u32 len)
{
	char *d = dest;
	const char *s = src;
	if (d < s)
	{
		while (len--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		const char *lasts = s + (len - 1);
		char *lastd = d + (len - 1);
		while (len--)
		{
			*lastd-- = *lasts--;
		}
	}

	return dest;
}
