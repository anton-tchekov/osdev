/**
 * @file    alloc.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    15.05.2023
 * @brief   Manual memory allocator
 */

#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <types.h>

/** Memory allocator information struct */
typedef struct
{
	i32 Used, Total, Size;
} MemAllocInfo;

/**
 * @brief Initialize the memory allocator. Do NOT call this function.
 *
 * @param heap_start Heap start address
 * @param size Maximum size of the heap
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

/**
 * @brief Returns memory allocator information and status
 *
 * @returns Pointer to memalloc info struct
 */
MemAllocInfo *memalloc_info(void);

#endif /* __ALLOC_H__ */
