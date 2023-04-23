/**
 * @file    ctype.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __CTYPE_H__
#define __CTYPE_H__

#include <types.h>

/* TODO: @param and @return */

/**
 * @brief This function checks whether the passed character is alphanumeric.
 */
i32 isalnum(i32 c);

/**
 * @brief This function checks whether the passed character is alphabetic.
 */
i32 isalpha(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a control character.
 */
i32 iscntrl(i32 c);

/**
 * @brief This function checks whether the passed character is decimal digit.
 */
i32 isdigit(i32 c);

/**
 * @brief This function checks whether the passed character has
 *        a graphical representation.
 */
i32 isgraph(i32 c);

/**
 * @brief This function checks whether the passed character is lowercase letter.
 */
i32 islower(i32 c);

/**
 * @brief This function checks whether the passed character is printable.
 */
i32 isprint(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a punctuation character.
 */
i32 ispunct(i32 c);

/**
 * @brief This function checks whether the passed character is white-space.
 */
i32 isspace(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        an uppercase letter.
 */
i32 isupper(i32 c);

/**
 * @brief This function checks whether the passed character is
 *        a hexadecimal digit.
 */
i32 isxdigit(i32 c);

/**
 * @brief This function converts uppercase letters to lowercase.
 */
i32 tolower(i32 c);

/**
 * @brief This function converts lowercase letters to uppercase.
 */
i32 toupper(i32 c);

#endif /* __CTYPE_H__ */
