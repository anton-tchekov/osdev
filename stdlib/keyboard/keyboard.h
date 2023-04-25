/**
 * @file    keyboard.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Keyboard input using event callbacks or polling
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <types.h>
#include <keys.h>

/** Keycode */
typedef u32 Key;

/**
 * @brief Check if a key is pressed
 *
 * @param key The key to check for (see `keys.h`)
 * @return Boolean
 */
bool keyboard_is_key_pressed(Key key);

/**
 * @brief Register an OnKeyboardInput event
 *
 * @param event Function pointer to a Function that takes
 *              the Key and the Bool up (true = key was released)
 */
void keyboard_register_event(void (*event)(Key key, bool up));

#endif /* __KEYBOARD_H__ */
