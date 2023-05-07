#include "types.h"

const char *keystate_string(KeyState state)
{
	const char *_keystate_strs[] =
	{
		"KEYSTATE_RELEASED",
		"KEYSTATE_PRESSED",
		"KEYSTATE_REPEAT"
	};

	return _keystate_strs[state];
}
