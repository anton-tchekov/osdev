#include <std.h>

void button_onclick(void)
{
	debug_print("Button OnClick\n");
}

Button button = BUTTON_CREATE(10, 10, 100, 30, "Click Me!", button_onclick);
Button button2 = BUTTON_CREATE(60, 300, 100, 30, "TEST 123 ...", NULL);


Label label_left = LABEL_CREATE(GFX_WIDTH / 2, 120, LABEL_FLAG_LEFT, "Align Left");
Label label_center = LABEL_CREATE(GFX_WIDTH / 2, 140, LABEL_FLAG_CENTER, "Align Center");
Label label_right = LABEL_CREATE(GFX_WIDTH / 2, 160, LABEL_FLAG_RIGHT, "Align Right");

char buf[50];

Input input = INPUT_CREATE(10, 200, 200, buf, sizeof(buf));


void *elements[] =
{
	&button,
	&label_left,
	&label_center,
	&label_right,
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

void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

void setup(void)
{
	keyboard_register_event(event_key);
	window_open(&window);
}

void loop(void)
{
}


