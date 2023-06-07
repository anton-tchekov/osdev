/**
 * @file    keyboard.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Keyboard input using event callbacks or polling
 */

#ifndef __KEYBOARD_USERSPACE_H__
#define __KEYBOARD_USERSPACE_H__

#include <types.h>
#include <keyboard-shared.h>

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
	return key & 0xFF;
}

/** Keyboard event handler function pointer */
typedef void (*KeyEvent)(Key, i32, KeyState);

/**
 * @brief Initialize keyboard, do NOT call this function.
 */
void keyboard_init(void);

/**
 * @brief Register an OnKeyboardInput event
 *
 * @param event Function pointer to a Function that takes
 *              the Key and the KeyState
 */
void keyboard_register_event(KeyEvent event);

#endif /* __KEYBOARD_USERSPACE_H__ */
