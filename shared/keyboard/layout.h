/**
 * @file    layout.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   Keyboard layout interface
 */

#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <types.h>
#include <keyboard-shared.h>

/**
 * @brief Convert key to codepoint
 *
 * @param k The scancode
 * @return Unicode codepoint
 */
i32 key_to_codepoint(Key k);

#endif /* __LAYOUT_H__ */
