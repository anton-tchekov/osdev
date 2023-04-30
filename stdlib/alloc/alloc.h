/**
 * @file    alloc.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Manual memory allocator
 */

#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <types.h>

/**
 * @brief Request a block of memory of `size` bytes
 *        This function will always return a valid pointer (never NULL).
 *        If the memory could not be allocated, the program will panic.
 *
 * @param size Number of bytes
 * @return Pointer to allocated memory
 */
void *malloc(u32 size);

/**
 * @brief Frees memory previously allocated with `malloc`
 *
 * @param p Pointer
 */
void free(void *p);

#endif /* __ALLOC_H__ */
