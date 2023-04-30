#include <std.h>

void button_onclick(void)
{
	debug_print("Button OnClick\n");
}

Button button = BUTTON_CREATE(10, 10, 100, 30, "Click Me!", button_onclick);
Button button2 = BUTTON_CREATE(60, 300, 100, 30, "TEST 123 ...", NULL);

Label label = LABEL_CREATE(10, 120, "Lorem ipsum dolor sit amet");

char buf[50];

Input input = INPUT_CREATE(10, 200, 200, buf, sizeof(buf));


void *elements[] =
{
	&button,
	&label,
	&input,
	&button2,
};

Window window =
{
	.Title = "Test",
	.Elements = elements,
	.Count = ARRLEN(elements),
	.OnKey = NULL
};

void event_key(Key key, KeyState down)
{
	debug_print("Key = %d, down = %d\n", key, down);
	window_event_key(key, down);
}

void setup(void)
{
	keyboard_register_event(event_key);
	window_open(&window);
}

void loop(void)
{
}


