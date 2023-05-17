#include <std.h>

static Window window;
static Button button, button2;
static Label label_left, label_center, label_right;
static Input input;

static void *elements[] =
{
	&button,
	&label_left,
	&label_center,
	&label_right,
	&input,
	&button2,
};

void button_onclick(void)
{
	debug_print("Button OnClick\n");
}

void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

void setup(void)
{
	string_test();

	keyboard_register_event(event_key);
	input_init(&input, 20, 220, 200);

	button_init(&button, 10, 10, 100, 30, "Click Me!", button_onclick);
	button_init(&button2, 60, 300, 100, 30, "TEST 123 ...", NULL);

	label_init(&label_left, GFX_WIDTH / 2, 120, LABEL_FLAG_LEFT, "Align Left");
	label_init(&label_center, GFX_WIDTH / 2, 140, LABEL_FLAG_CENTER, "Align Center");
	label_init(&label_right, GFX_WIDTH / 2, 160, LABEL_FLAG_RIGHT, "Align Right");

	window_init(&window, "Test", elements, ARRLEN(elements), NULL);
	window_open(&window);
}

void loop(void)
{
}


