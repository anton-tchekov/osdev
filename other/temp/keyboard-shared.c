/**
 * @file    keyboard-shared.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 */

#include <keyboard-shared.h>

const char *keystate_str(KeyState state)
{
	const char *_keystate_strs[] =
	{
		"KEYSTATE_RELEASED",
		"KEYSTATE_PRESSED",
		"KEYSTATE_REPEAT"
	};

	return _keystate_strs[state];
}
