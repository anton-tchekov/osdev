/**
 * @file    keyword.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    28.05.2023
 * @brief   Keyword lookup function for syntax highlighting
 */

#ifndef __KEYWORD_H__
#define __KEYWORD_H__

#include <types.h>

/**
 * @brief Initialize keyword module
 */
void keyword_init(void);

/**
 * @brief Detects if a string is a keyword
 *
 * @param str The string to check for
 * @param len The length of the string
 * @return true if the input string is a keyword
 */
bool keyword_detect(const char *str, u32 len);

#endif /* __KEYWORD_H__ */
