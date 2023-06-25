#include "strcpy.h"

char *strcpy(char *dest, const char *src)
{
	char *d = dest;
	while((*d++ = *src++)) {}
	return dest;
}

char *strncpy(char *dest, const char *src, u32 count)
{
	u32 i, c;
	for(i = 0; i < count && (c = src[i]); ++i)
	{
		dest[i] = c;
	}

	return dest;
}
