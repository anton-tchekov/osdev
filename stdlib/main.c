#include <std.h>

i32 main(void)
{
	// serial_write("hello world!\n", 13);
	char str[] = "A";
	font_string(10, 10, str, font_default, COLOR_WHITE, COLOR_BLACK);

	loop {}
	return 0;
}
