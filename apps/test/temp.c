
static char *reverse(char *s)
{
	i32 i, j, tmp;
	for(i = 0, j = strlen(s) - 1; i < j; ++i, --j)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	return s;
}

reverse(str);

i32 is_prime(i32 v)
{
	i32 i;
	for(i = 2; i <= v / 2; ++i)
	{
		if(v % i == 0)
		{
			return 0;
		}
	}

	return 1;
}

/*
	u32 begin, end;
	i32 i, count;

	debug_print("Start!\n");

	begin = millis();
	debug_print("begin = %d\n", 0);

	count = 0;
	for(i = 2; i <= 10000; ++i)
	{
		debug_print("i = %d\n", i);
		if(is_prime(i))
		{
			++count;
			debug_print("prime! count = %d\n", count);
		}
	}

	end = millis();
	debug_print("Number of primes: %d - Time spent: %d ms\n",
		count, end - begin);
*/