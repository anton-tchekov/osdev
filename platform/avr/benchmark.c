/**
 * @file    benchmark.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   Simple CPU benchmark
 */

static u8 is_prime(u16 v)
{
	u16 i;
	for(i = 2; i < v / 2; ++i)
	{
		if(v % i == 0)
		{
			return 0;
		}
	}

	return 1;
}

static void benchmark(void)
{
	u16 i, count;
	u32 end, start;

	start = millis();
	count = 0;
	for(i = 3; i <= 1000; ++i)
	{
		if(is_prime(i))
		{
			++count;
		}
	}
	end = millis();

	log_boot_P(LOG_EXT, PSTR("\nNumber of primes: %"PRIu16""), count);
	log_boot_P(LOG_EXT, PSTR("CPU bench: %"PRIu16" ms\n"), end - start);

	start = millis();
	for(i = 0; i < 32; ++i)
	{
		xmem_set(0, 0, 0, 0x8000);
		xmem_set(0, 0x8000, 0, 0x8000);
	}
	end = millis();

	log_boot_P(LOG_EXT, PSTR("memset 32x 64K => %"PRIu16" ms\n"), end - start);
}
