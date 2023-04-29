/**
 * @file    keyboard.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <keyboard.h>
#include <syscall.h>
#include <event.h>

static void (*_event_key)(Key, bool) = NULL;

static void _event_key_handler(Key key, bool down)
{
	if(_event_key)
	{
		_event_key(key, down);
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

void keyboard_register_event(void (*event)(Key key, bool down))
{
	_event_key = event;
}
