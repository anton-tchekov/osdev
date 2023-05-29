/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 * @brief   Init program
 */

#include <std.h>

/* --- CONSTANTS --- */

/** Width of all login input fields */
#define INPUT_WIDTH        220

/** Width of the login button */
#define BUTTON_WIDTH       140

/** Height of the login button */
#define BUTTON_HEIGHT       28

/** Y-coordinate of the username input */
#define INPUT_USERNAME_Y   110

/** Y-coordinate of the password input */
#define INPUT_PASSWORD_Y   160

/** Y-coordinate of the login button */
#define BUTTON_LOGIN_Y     204

/** Offset of a label belonging to an input */
#define INPUT_LABEL_OFFSET  16

/** Duration for which to show the error */
#define SHOW_ERROR_DELAY  2000

/** Blink interval */
#define BLINK_DELAY        300

/** Error label Y-coordinate */
#define LABEL_ERROR_Y       60

/** No Apps label Y-coordinate */
#define LABEL_NO_APPS_Y     50

/* --- VARIABLES --- */

/** Login window with username and password field and login button */
static Window _window_login;

/** Apps window with icons */
static Window _window_apps;

/** Login button */
static Button _button_login;

/** Label for username input field */
static Label _label_username;

/** Label for password input field */
static Label _label_password;

/** Label to indicate that there are no installed apps */
static Label _label_no_apps;

/** Blinking error label for when password is wrong */
static Label _label_response;

/** Username input field */
static Input _input_username;

/** Password input field (characters are hidden with asterisks) */
static Input _input_password;

/** Timestamp when the error occured */
static u32 _millis_error;

/** TImestamp since the last blink */
static u32 _millis_blink;

/** Boolean indicator that there is an error */
static bool _error;

/** Boolean indicating that the label is visible */
static bool _visible;

/** Elements array of the login window */
static void *elements_login[] =
{
	&_label_response,
	&_label_username,
	&_input_username,
	&_label_password,
	&_input_password,
	&_button_login,
};

/** Elements array of the apps window */
static void *elements_apps[] =
{
	&_label_no_apps
};

/* --- FUNCTIONS --- */

/**
 * @brief Show wrong password error
 */
static void _show_error(void)
{
	_error = true;
	_visible = true;
	_millis_blink = _millis_error = millis();
	label_show(&_label_response, true);
}

/**
 * @brief Hide wrong password error
 */
static void _hide_error(void)
{
	_error = false;
	label_show(&_label_response, false);
}

/**
 * @brief Login button onclick and form enter submit handler
 */
static void button_login_onclick(void)
{
	/**
	 * Yes I know calling this is a bad practice is an understatement.
	 * I apologise in advance to the programming gods for hardcoding
	 * credentials in plain text.
	 *
	 * The whole point of this is to showcase the GUI and have it look cool.
	 */
	if(!strncmp(vector_data(&_input_username.Text), "Anton", 5) &&
		!strncmp(vector_data(&_input_password.Text), "OSDEV1234", 9))
	{
		window_open(&_window_apps);
	}
	else
	{
		_show_error();
	}
}

/**
 * @brief Window key event handler. Submit form on enter pressed
 *
 * @param key The key that was pressed
 * @param chr Character corresponding to the key
 * @param state Current key state
 */
static void window_login_onkey(Key key, i32 chr, KeyState state)
{
	if(key == KEY_RETURN && state == KEYSTATE_PRESSED)
	{
		button_login_onclick();
	}

	(void)chr;
}

/**
 * @brief Main key event handler
 *
 * @param key The key that was pressed
 * @param chr The character corresponding to the key
 * @param state Key state (pressed, repeat, released)
 */
static void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

/**
 * @brief Main setup function, called on program startup
 */
void setup(void)
{
	keyboard_register_event(event_key);

	label_init(&_label_response,
		GFX_WIDTH / 2,
		LABEL_ERROR_Y,
		FLAG_ALIGN_CENTER | FLAG_INVERTED | FLAG_INVISIBLE,
		"Wrong username or password");

	label_init(&_label_username,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_USERNAME_Y - INPUT_LABEL_OFFSET,
		FLAG_ALIGN_LEFT,
		"Username:");

	input_init(&_input_username,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_USERNAME_Y,
		INPUT_WIDTH);

	label_init(&_label_password,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_PASSWORD_Y - INPUT_LABEL_OFFSET,
		FLAG_ALIGN_LEFT,
		"Password:");

	input_init(&_input_password,
		GFX_WIDTH / 2 - INPUT_WIDTH / 2,
		INPUT_PASSWORD_Y,
		INPUT_WIDTH);

	_input_password.Flags |= FLAG_PASSWORD;

	button_init(&_button_login,
		GFX_WIDTH / 2 - BUTTON_WIDTH / 2,
		BUTTON_LOGIN_Y,
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		"[ Login ]",
		button_login_onclick);

	label_init(&_label_no_apps,
		GFX_WIDTH / 2,
		LABEL_NO_APPS_Y,
		FLAG_ALIGN_CENTER,
		"No apps installed");

	window_init(&_window_apps, "Apps", elements_apps,
		ARRLEN(elements_apps), NULL);

	window_init(&_window_login, "Login", elements_login,
		ARRLEN(elements_login), window_login_onkey);

	window_open(&_window_login);
}

/**
 * @brief Main loop function, called repeatedly
 */
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
			label_show(&_label_response, _visible);
			_millis_blink = now;
		}
	}
}
