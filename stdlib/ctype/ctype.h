/**
 * @file    ctype.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Standard C functions to check for and convert between
 *          ASCII characters
 */

#ifndef __CTYPE_H__
#define __CTYPE_H__

#include <types.h>

/**
 * @brief This function checks whether the passed character is alphanumeric.
 *
 * @param c The character
 * @return Boolean
 */
bool isalnum(i32 c);

/**
 * @brief This function checks whether the passed character is alphabetic.
 *
 * @param c The character
 * @return Boolean
 */
bool isalpha(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a control character.
 *
 * @param c The character
 * @return Boolean
 */
bool iscntrl(i32 c);

/**
 * @brief This function checks whether the passed character is decimal digit.
 *
 * @param c The character
 * @return Boolean
 */
bool isdigit(i32 c);

/**
 * @brief This function checks whether the passed character has
 *        a graphical representation.
 *
 * @param c The character
 * @return Boolean
 */
bool isgraph(i32 c);

/**
 * @brief This function checks whether the passed character is lowercase letter.
 *
 * @param c The character
 * @return Boolean
 */
bool islower(i32 c);

/**
 * @brief This function checks whether the passed character is printable.
 *
 * @param c The character
 * @return Boolean
 */
bool isprint(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a punctuation character.
 *
 * @param c The character
 * @return Boolean
 */
bool ispunct(i32 c);

/**
 * @brief This function checks whether the passed character is white-space.
 *
 * @param c The character
 * @return Boolean
 */
bool isspace(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        an uppercase letter.
 *
 * @param c The character
 * @return Boolean
 */
bool isupper(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a hexadecimal digit.
 *
 * @param c The character
 * @return Boolean
 */
bool isxdigit(i32 c);

/**
 * @brief This function converts uppercase letters to lowercase.
 *
 * @param c The character
 * @return The character converted to lowercase.
 *         If the input was not uppercase, returns the same character
 */
i32 tolower(i32 c);

/**
 * @brief This function converts lowercase letters to uppercase.
 *
 * @param c The character
 * @return The character converted to uppercase.
 *         If the input was not lowercase, returns the same character
 */
i32 toupper(i32 c);

#endif /* __CTYPE_H__ */
