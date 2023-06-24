
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