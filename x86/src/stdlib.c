#include "stdlib.h"

u32 strlen(const char *str)
{
	const char *p = str;
	while (*p++)
		;
	return p - str - 1;
}

void memset8(u8 *ptr, u8 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

void memset16(u16 *ptr, u16 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

void memset32(u32 *ptr, u32 value, u32 count)
{
	while(count--)
	{
		*ptr++ = value;
	}
}

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

void strrev(char *str)
{
	u32 i;
	u32 j;
	char a;
	u32 len = strlen((const char *)str);
	for (i = 0, j = len - 1; i < j; i++, j--)
	{
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

int itoa(int num, char *str, int base)
{
	int sum = num;
	int digit;
	int i = 0;
	do
	{
		digit = sum % base;
		if (digit < 0xA)
			str[i++] = '0' + digit;
		else
			str[i++] = 'A' + digit - 0xA;
		sum /= base;
	} while (sum);

	str[i] = '\0';
	strrev(str);
	return 0;
}