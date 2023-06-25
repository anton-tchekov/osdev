#include "strchr.h"

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
