/**
 * @file    string.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Standard C routines that operate on strings of memory
 */

#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>

/**
 * @brief Copies `count` bytes from `src` to `dest`
 *        Source and destination blocks must NOT overlap.
 *
 * @param dest Pointer to destination address
 * @param src Pointer to source address
 * @param count Number of bytes
 * @return `dest`
 */
void *memcpy(void *dest, const void *src, u32 count);

/**
 * @brief Copies `count` bytes from `src` to `dest`
 *        Source and destination blocks are allowed to overlap.
 *
 * @param dest Pointer to destination address
 * @param src Pointer to source address
 * @param count Number of bytes
 * @return `dest`
 */
void *memmove(void *dest, const void *src, u32 count);

/**
 * @brief Compares two blocks of memory
 *
 * @param ptr1 TODO
 * @param ptr2 TODO
 * @param count TODO
 * @return TODO
 */
i32 memcmp(const void *ptr1, const void *ptr2, u32 count);

/**
 * @brief Finds a char in
 *
 * @param ptr TODO
 * @param value TODO
 * @param count TODO
 * @return TODO
 */
const void *memchr(const void *ptr, i32 value, u32 count);

/**
 * @brief Sets all bytes in a block of memory to `value`
 *
 * @param ptr Pointer to block of memory
 * @param value Value to be written as byte
 * @param count Number of bytes
 * @return `ptr`
 */
void *memset(void *ptr, i32 value, u32 count);

/**
 * @brief TODO
 *
 * @param dest TODO
 * @param src TODO
 * @return TODO
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief TODO
 *
 * @param dest TODO
 * @param src TODO
 * @param count TODO
 * @return TODO
 */
char *strncpy(char *dest, const char *src, u32 count);

/**
 * @brief TODO
 *
 * @param str TODO
 * @return TODO
 */
u32 strlen(const char *str);

/**
 * @brief TODO
 *
 * @param str1 TODO
 * @param str2 TODO
 * @return TODO
 */
i32 strcmp(const char *str1, const char *str2);

/**
 * @brief TODO
 *
 * @param str1 TODO
 * @param str2 TODO
 * @param count TODO
 * @return TODO
 */
i32 strncmp(const char *str1, const char *str2, u32 count);

/**
 * @brief TODO
 *
 * @param str TODO
 * @param c TODO
 * @return TODO
 */
const char *strchr(const char *str, i32 c);

#endif /* __STRING_H__ */
