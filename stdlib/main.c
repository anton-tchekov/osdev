#include <std.h>
#include <arial.h>
#include <fatty.h>

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

Button button = BUTTON_CREATE(10, 10, 100, 30, "Click Me!", NULL);
Button button2 = BUTTON_CREATE(60, 300, 100, 30, "TEST 123 ...", NULL);

Label label = LABEL_CREATE(10, 120, "Lorem ipsum dolor sit amet");

void *elements[] =
{
	&button,
	&label,
	&button2,
};

Window window =
{
	.Title = "Test",
	.Elements = elements,
	.Count = ARRLEN(elements),
	.OnKey = NULL
};

i32 main(void)
{
	window_open(&window);

	char str[] = "Hello World!";
	font_string(10, 80, str, arial, COLOR_BLACK, COLOR_WHITE);



font_string(30, 260, "A", fatty, COLOR_BLACK, COLOR_WHITE);

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

	loop {}
	return 0;
}
