/**
 * @file    keyboard.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <keyboard.h>
#include <syscall.h>
#include <event.h>

/** Pointer to user-defined key event handler function */
static KeyEvent _event_key;

/** Wrapper for key event handler */
static void _event_key_handler(Key key, i32 chr, KeyState state)
{
	if(_event_key)
	{
		_event_key(key, chr, state);
	}

	finish();
}

void keyboard_init(void)
{
	event_register(EVENT_KEY, (u32)_event_key_handler);
}

bool keyboard_is_key_pressed(Key key)
{
	return syscall1(SYSCALL_KEYBOARD_IS_KEY_PRESSED, key);
}

void keyboard_register_event(KeyEvent event)
{
	_event_key = event;
}
