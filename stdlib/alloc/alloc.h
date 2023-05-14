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
 * @brief Initialize the memory allocator
*/
void memalloc_init(ptr heap_start, i32 size);

/**
 * @brief Request a block of memory of `size` bytes
 *        This function will always return a valid pointer (never NULL).
 *        If the memory could not be allocated, the program will panic.
 *
 * @param size Number of bytes
 * @return Pointer to allocated memory
 */
void *memalloc(u32 size);

/**
 * @brief Frees memory previously allocated with `memalloc`
 *
 * @param p Pointer
 */
void memfree(void *p);

void print_chain(void);
void print_stats(void);

#endif /* __ALLOC_H__ */
