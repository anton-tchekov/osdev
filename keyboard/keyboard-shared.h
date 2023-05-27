/**
 * @file    keyboard-shared.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   Keyboard types and utilities
 */

#ifndef __KEYBOARD_SHARED_H__
#define __KEYBOARD_SHARED_H__

#include <types.h>
#include <keys.h>

/** Key State: Released (Key Up), Pressed (Key Down), Repeat (Held Down) */
typedef enum
{
	KEYSTATE_RELEASED,
	KEYSTATE_PRESSED,
	KEYSTATE_REPEAT
} KeyState;

/** Keycode */
typedef u32 Key;

/**
 * @brief Remove modifiers from key
 *
 * @param key Scancode
 * @return Scancode with modifiers removed
 */
static inline Key key_mod_remove(Key key)
{
	return key & 0xFFFF;
}

/**
 * @brief Get a textual representation of a key state
 *
 * @param state Key state (see enum above)
 * @return String
 */
const char *keystate_string(KeyState state);

#endif /* __KEYBOARD_SHARED_H__ */
