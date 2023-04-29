#include <std.h>
#include <ubuntu_bold.h>

Window window =
	{
		.Title = "Test",
		.OnKey = NULL};

void clear()
{
	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
}

void sleep(i32 mills)
{
	i32 begin = millis();
	while (millis() - begin < mills)
	{
	}
	return;
}

i32 main(void)
{
	window_open(&window);

	char str[] = "Game";

	i32 str_w = font_string_width(str, ubuntu_bold);
	i32 str_h = ubuntu_bold->Size;

	i32 h_center = GFX_WIDTH / 2;
	i32 x_move = h_center - (str_w / 2);

	i32 v_center = GFX_HEIGHT / 2;
	i32 y_move = v_center - (str_h / 2);

	clear();

	loop
	{
		if (keyboard_is_key_pressed(KEY_W))
		{
			y_move -= 1;
			clear();
		}
		font_string(x_move, y_move, str, ubuntu_bold, COLOR_WHITE, COLOR_BLACK);
		sleep(100);
	}

	return 0;
}
