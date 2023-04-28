#include <std.h>

i32 main(void)
{
	// serial_write("hello world!\n", 13);

	char str[] = "hello world\n";
	font_string(10, 10, str, font_default, COLOR_WHITE, COLOR_BLACK);

	while(1)
	{
		//gfx_rect(10, 10, 200, 200, gfx_color(0xFF, 0x00, 0x00, 0x00));
	}
	return 0;
}
