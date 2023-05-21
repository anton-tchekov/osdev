#include <std.h>

static Window window;
static Button button_login;
static Label label_username, label_password;
static Input input_username, input_password;

static void *elements[] =
{
	&label_username,
	&input_username,
	&label_password,
	&input_password,
	&button_login,
};

void button_login_onclick(void)
{
	debug_print("Login button pressed\n");
}

void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

#define INPUT_WIDTH        220
#define BUTTON_WIDTH       140
#define BUTTON_HEIGHT       28
#define INPUT_USERNAME_Y   110
#define INPUT_PASSWORD_Y   160
#define BUTTON_LOGIN_Y     204
#define INPUT_LABEL_OFFSET  16

void setup(void)
{
	string_test();
	keyboard_register_event(event_key);

	label_init(&label_username,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_USERNAME_Y - INPUT_LABEL_OFFSET,
		FLAG_ALIGN_LEFT,
		"Username:");

	input_init(&input_username,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_USERNAME_Y,
		INPUT_WIDTH);

	label_init(&label_password,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_PASSWORD_Y - INPUT_LABEL_OFFSET,
		FLAG_ALIGN_LEFT,
		"Password:");

	input_init(&input_password,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_PASSWORD_Y,
		INPUT_WIDTH);

	button_init(&button_login,
		GFX_WIDTH / 2 - BUTTON_WIDTH / 2,
		BUTTON_LOGIN_Y,
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		"[ Login ]",
		button_login_onclick);

	window_init(&window, "Login", elements, ARRLEN(elements), NULL);
	window_open(&window);
}

void loop(void)
{
}
