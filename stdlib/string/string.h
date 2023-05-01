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
 * @brief Compares the first n bytes of two memory blocks pointed to by the
 *        provided pointers.
 *
 * @param ptr1 A pointer to the first memory block.
 * @param ptr2 A pointer to the second memory block.
 * @param n The number of bytes to compare.
 * @return An integer less than, equal to, or greater than zero, indicating
 *         whether the first n bytes of the memory blocks pointed to by ptr1
 *         and ptr2 are less than, equal to, or greater than each other,
 *         respectively.
 */
i32 memcmp(const void *ptr1, const void *ptr2, u32 count);

/**
 * @brief Searches for the first occurrence of a specific character in the
 *        first n bytes of the memory block pointed to by the provided pointer.
 *
 * @param ptr A pointer to the memory block to be searched.
 * @param value The value of the character to be searched for.
 * @param n The number of bytes to be searched.
 * @return A pointer to the first occurrence of the character, if found, or a
 *         null pointer if the character is not found in the first n bytes of
 +         the memory block.
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
 * @brief Copy a string from source to destination
 *
 * @param dest Pointer to the destination string
 * @param src Pointer to the source string
 * @return Pointer to the destination string
 *
 * This function copies the content of the null-terminated string pointed to by
 * `src` to the buffer pointed to by `dest`, including the null terminator.
 */
char *strcpy(char *dest, const char *src);

/**
 * @brief Copy a limited number of characters from one string to another
 *
 * @param dest Pointer to the destination array
 * @param src Pointer to the source of data
 * @param n Maximum number of characters to be copied
 * @return Pointer to the destination array
 */
char *strncpy(char *dest, const char *src, u32 count);

/**
 * @brief Get the length of a string.
 *
 * @param str Pointer to the null-terminated string to be measured.
 * @return Length of the string.
 *
 * This function calculates the length of the null-terminated string pointed
 * to by `str`, not including the null terminator.
 */
u32 strlen(const char *str);

/**
 * @brief Compare two strings.
 *
 * @param str1 Pointer to the first null-terminated string to be compared.
 * @param str2 Pointer to the second null-terminated string to be compared.
 * @return A negative value if the first character that does not match has a
 *         lower value in `str1` than in `str2`, a positive value if the first
 *         character that does not match has a greater value in `str1` than in
 *         `str2`, or zero if the contents of both strings are equal.
 */
i32 strcmp(const char *str1, const char *str2);

/**
 * @brief Compare a limited number of characters in two strings.
 *
 * @param str1 Pointer to the first null-terminated string to be compared.
 * @param str2 Pointer to the second null-terminated string to be compared.
 * @param n Maximum number of characters to be compared.
 * @return A negative value if the first character that does not match has a
 *         lower value in `str1` than in `str2`, a positive value if the first
 *         character that does not match has a greater value in `str1` than in
 *         `str2`, or zero if the contents of both strings are equal up to the
 *         first `n`
 */
i32 strncmp(const char *str1, const char *str2, u32 count);

/**
 * @brief Locate the first occurrence of a character in a string.
 *
 * @param str Pointer to the null-terminated string to be searched.
 * @param c Character to be located.
 * @return Pointer to the first occurrence of `c` in `str`,
 *         or NULL if `c` is not found.
 *
 * This function searches for the first occurrence of the character `c` in the
 * null-terminated string pointed to by `str`. If `c` is found in `str`, the
 * function returns a pointer to the first occurrence of `c` in `str`. If `c`
 * is not found in `str`, the function returns NULL.
 */
const char *strchr(const char *str, i32 c);

#endif /* __STRING_H__ */
