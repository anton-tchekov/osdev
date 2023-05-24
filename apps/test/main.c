/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 *
 * @brief   Init program
 */

#include <std.h>

#define INPUT_WIDTH        220
#define BUTTON_WIDTH       140
#define BUTTON_HEIGHT       28
#define INPUT_USERNAME_Y   110
#define INPUT_PASSWORD_Y   160
#define BUTTON_LOGIN_Y     204
#define INPUT_LABEL_OFFSET  16
#define SHOW_ERROR_DELAY  2000
#define BLINK_DELAY        300
#define LABEL_ERROR_Y       60
#define LABEL_NO_APPS_Y     50

static Window window_login, window_apps;
static Button button_login;
static Label label_username, label_password, label_no_apps, label_response;
static Input input_username, input_password;
static u32 _millis_error, _millis_blink;
static bool _error, _visible;

static void *elements_login[] =
{
	&label_response,
	&label_username,
	&input_username,
	&label_password,
	&input_password,
	&button_login,
};

static void *elements_apps[] =
{
	&label_no_apps
};

static void _show_error(void)
{
	_error = true;
	_visible = true;
	_millis_blink = _millis_error = millis();
	label_show(&label_response, true);
}

static void _hide_error(void)
{
	_error = false;
	label_show(&label_response, false);
}

static void button_login_onclick(void)
{
	/**
	 * Yes I know calling this is a bad practice is an understatement.
	 * I apologise in advance to the programming gods for hardcoding
	 * credentials in plain text.
	 *
	 * The whole point of this is to showcase the GUI and have it look cool.
	 */
	if(!strncmp(vector_data(&input_username.Text), "Anton", 5) &&
		!strncmp(vector_data(&input_password.Text), "OSDEV1234", 9))
	{
		window_open(&window_apps);
	}
	else
	{
		_show_error();
	}
}

static void window_login_onkey(Key key, i32 chr, KeyState state)
{
	if(key == KEY_RETURN && state == KEYSTATE_PRESSED)
	{
		button_login_onclick();
	}

	(void)chr;
}

void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

void setup(void)
{
	keyboard_register_event(event_key);

	label_init(&label_response,
		GFX_WIDTH / 2,
		LABEL_ERROR_Y,
		FLAG_ALIGN_CENTER | FLAG_INVERTED | FLAG_INVISIBLE,
		"Wrong username or password");

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

	input_password.Flags |= FLAG_PASSWORD;

	button_init(&button_login,
		GFX_WIDTH / 2 - BUTTON_WIDTH / 2,
		BUTTON_LOGIN_Y,
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		"[ Login ]",
		button_login_onclick);

	label_init(&label_no_apps,
		GFX_WIDTH / 2,
		LABEL_NO_APPS_Y,
		FLAG_ALIGN_CENTER,
		"No apps installed");

	window_init(&window_apps, "Apps", elements_apps,
		ARRLEN(elements_apps), NULL);

	window_init(&window_login, "Login", elements_login,
		ARRLEN(elements_login), window_login_onkey);

	window_open(&window_login);
}

void loop(void)
{
	if(_error)
	{
		u32 now = millis();
		if(now > _millis_error + SHOW_ERROR_DELAY)
		{
			_hide_error();
		}
		else if(now > _millis_blink + BLINK_DELAY)
		{
			_visible = !_visible;
			label_show(&label_response, _visible);
			_millis_blink = now;
		}
	}
}
