#include <std.h>
#include <arial.h>
#include <fatty.h>
#include <ubuntu_bold.h>

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

void event_key(Key key, bool down)
{
	debug_print("Key = %d, down = %d\n", key, down);

	if(key == KEY_W)
	{
		gfx_rect(GFX_WIDTH / 2, GFX_HEIGHT / 2,
			10, 10, COLOR_BLUE);
	}
}

void setup(void)
{
	keyboard_register_event(event_key);

	window_open(&window);

	font_string(10, 80, "Hello World!", arial, COLOR_BLACK, COLOR_WHITE);
	font_string(30, 260, "A", fatty, COLOR_BLACK, COLOR_WHITE);
	font_string(30, 360, "Alles klar warum nicht?", ubuntu_bold, COLOR_BLACK, COLOR_WHITE);
}

int y = 0;

void loop(void)
{
	gfx_rect(0, y, 10, 10, COLOR_RED);
	y += 10;
}


