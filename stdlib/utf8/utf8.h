/**
 * @file    utf8.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   UTF-8 String Handling
 */

#ifndef __UTF8_H__
#define __UTF8_H__

#include <types.h>

/**
 * @brief Gets the unicode codepoint of the first
 *        character in a UTF-8 string.
 *
 * @param s The string
 * @param out Output parameter for codepoint
 * @return Pointer to the string without the first character
 */
const char *utf8(const char *s, i32 *out);

#endif /* __UTF8_H__ */
