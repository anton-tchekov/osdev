#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"
#include "keys.h"

typedef u32 Key;

/*
* @brief Check if a Key is pressed
* @param key The Key to check (see 'keys.h')
* @return Boolean  
*/
bool keyboard_is_key_pressed(Key key);

/*
* @brief Register an OnKeyboardInput event
* @param event Function pointer to a Function that tages the Key and the Bool up (true = key was released)
*/
void keyboard_register_event(void (*event)(Key key, bool up));

#endif /* __KEYBOARD_H__ */