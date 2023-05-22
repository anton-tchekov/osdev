#include <std.h>

void event_key(Key key, i32 chr, KeyState state)
{
	window_event_key(key, chr, state);
}

void setup(void)
{
	keyboard_register_event(event_key);
}

void loop(void)
{
}
