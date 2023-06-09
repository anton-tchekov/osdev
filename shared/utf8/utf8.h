/**
 * @file    utf8.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.2
 * @date    16.05.2023
 * @brief   UTF-8 String Handling
 */

#ifndef __UTF8_H__
#define __UTF8_H__

#include <types.h>

/**
 * @brief Gets the UTF-8 string from a unicode codepoint.
 *
 * @param codepoint Unicode codepoint
 * @param out Output parameter for UTF-8 string
 * @return Number of bytes written or 0 on error
 */
i32 codepoint_utf8(i32 codepoint, char *out);

/**
 * @brief Gets the unicode codepoint of the first
 *        character in a UTF-8 string.
 *
 * @param s The string
 * @param out Output parameter for codepoint
 * @return Pointer to the string without the first character
 */
const char *utf8_codepoint(const char *s, i32 *out);

/**
 * @brief Gets the number of unicode codepoints in a UTF-8 string.
 *
 * @param s The string
 * @return Number of codepoints in the string
 */
u32 utf8_length(const char *s);

#endif /* __UTF8_H__ */
