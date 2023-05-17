/**
 * @file    string.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    17.05.2023
 */

#include <string.h>
#include <syscall.h>

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

u32 strlen(const char *str)
{
	const char *s = str;
	while(*s) { ++s; }
	return s - str;
}

i32 strcmp(const char *str1, const char *str2)
{
	while(*str1 == *str2)
	{
		if(!*str1)
		{
			return 0;
		}

		++str1;
		++str2;
	}

	return *str1 - *str2;
}

i32 strncmp(const char *str1, const char *str2, u32 count)
{
	u32 i;
	const u8 *p1, *p2;

	p1 = (const u8 *)str1;
	p2 = (const u8 *)str2;
	for(i = 0; i < count; ++i)
	{
		if(*p1 != *p2)
		{
			return *p1 - *p2;
		}

		if(!*p1)
		{
			return 0;
		}

		++p1;
		++p2;
	}

	return 0;
}

const char *strchr(const char *str, i32 c)
{
	i32 d;
	while((d = *str))
	{
		if(d == c)
		{
			return str;
		}

		++str;
	}

	return NULL;
}

void *memcpy(void *dest, const void *src, u32 count)
{
	u8 *d, *s, *end;

	d = (u8 *)src;
	s = (u8 *)dest;
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

void *memmove(void *dest, const void *src, u32 count)
{
	u8 *d, *s, *end;

	s = (u8 *)src;
	d = (u8 *)dest;
	end = s + count;

	if(d > s && d < end)
	{
		s = end;
		d += count;
		while(s > (u8 *)src)
		{
			*--d = *--s;
		}
	}
	else if(s != d)
	{
		while(s < end)
		{
			*d++ = *s++;
		}
	}

	return dest;
}

i32 memcmp(const void *ptr1, const void *ptr2, u32 count)
{
	u32 i;
	const u8 *p1, *p2;

	p1 = (const u8 *)ptr1;
	p2 = (const u8 *)ptr2;
	for(i = 0; i < count; ++i)
	{
		if(*p1 != *p2)
		{
			return *p1 - *p2;
		}

		++p1;
		++p2;
	}

	return 0;
}

const void *memchr(const void *ptr, i32 value, u32 count)
{
	u8 *p, *end;
	for(p = (u8 *)ptr, end = p + count; p < end; ++p)
	{
		if(*p == value)
		{
			return p;
		}
	}

	return NULL;
}

void *memset(void *ptr, i32 value, u32 count)
{
	u8 *p, *end;
	for(p = (u8 *)ptr, end = p + count; p < end; ++p)
	{
		*p = value;
	}

	return ptr;
}
