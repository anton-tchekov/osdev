#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"
#include "keys.h"

typedef enum
{
	KEYSTATE_PRESSED,
	KEYSTATE_RELEASED
} KeyState;

typedef void (*KeyEvent)(Key, i32, KeyState);

void keyboard_init(void);
void keyboard_event_register(KeyEvent event);

#endif /* __KEYBOARD_H__ */
