#include "memset.h"

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
