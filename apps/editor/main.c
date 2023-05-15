#include <std.h>

#include "editor.h"

static Editor ed;

void event_key(Key key, i32 chr, KeyState state)
{
	if(state == KEYSTATE_RELEASED)
	{
		return;
	}

	if(isprint(chr))
	{
		editor_char(&ed, chr);
	}
	else if(key == KEY_BACKSPACE)
	{
		editor_backspace(&ed);
	}
	else if(key == KEY_DELETE)
	{
		editor_delete(&ed);
	}
	else if(key == KEY_HOME)
	{
		editor_home(&ed);
	}
	else if(key == KEY_END)
	{
		editor_end(&ed);
	}
	else if(key == KEY_PAGEUP)
	{
		editor_page_up(&ed);
	}
	else if(key == KEY_PAGEDOWN)
	{
		editor_page_down(&ed);
	}
	else if(key == KEY_LEFT)
	{
		editor_left(&ed);
	}
	else if(key == KEY_RIGHT)
	{
		editor_right(&ed);
	}
	else if(key == KEY_UP)
	{
		editor_up(&ed);
	}
	else if(key == KEY_DOWN)
	{
		editor_down(&ed);
	}
}

void setup(void)
{
	keyboard_register_event(event_key);
	editor_init(&ed);
}

void loop(void)
{
}
