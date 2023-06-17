#include <std.h>

/**
 * @brief Check if a number is a prime
 *
 * @param v The number to check
 * @return Boolean true if prime, else false
 */
bool is_prime(i32 v)
{
	i32 i;
	for(i = 2; i <= v / 2; ++i)
	{
		if(v % i == 0)
		{
			return false;
		}
	}

	return true;
}

/**
 * @brief Main setup function, called on program startup
 */
void setup(void)
{
	u32 begin, end;
	i32 i, count;

	debug_print("Start!\n");
	begin = millis();
	count = 0;
	for(i = 2; i <= 1000; ++i)
	{
		if(is_prime(i))
		{
			++count;
		}
	}

	end = millis();
	debug_print("Number of primes: %d - Time spent: %d ms\n",
		count, end - begin);
}

/**
 * @brief Main loop function, called repeatedly
 */
void loop(void)
{
}
