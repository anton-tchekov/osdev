#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"
#include "keys.h"

typedef enum
{
	KEYSTATE_PRESSED,
	KEYSTATE_RELEASED
} KeyState;

void key_event(Key key, i32 scancode, KeyState state);
void keyboard_init(void);

#endif /* __KEYBOARD_H__ */
