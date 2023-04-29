#include <std.h>
#include <ubuntu_bold.h>
#include "assets/bootscreen.c"

Window window =
{
	.Title = "Test",
	.OnKey = NULL
};

i32 main(void)
{
	window_open(&window);

	char str[] = "";
	i32 h_center = GFX_WIDTH / 2;
	i32 x_move = h_center - (bootscreen.width / 2);

	i32 v_center = GFX_HEIGHT / 2;
	i32 y_move = v_center - (bootscreen.height / 2);
	
	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
	gfx_image_rgb(x_move, y_move, bootscreen.width, bootscreen.height, bootscreen.pixel_data);

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
Your branch is up to date with 'origin/main'.
*/

	i32 begin = millis();

	while (millis() - begin < 2000) {}


	return 0;
}
