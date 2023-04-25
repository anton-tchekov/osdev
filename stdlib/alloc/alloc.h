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
 * @brief TODO
 *
 * @param p TODO
 */
void free(void *p);

/**
 * @brief TODO
 *
 * @param size TODO
 * @return TODO
 */
void *malloc(u32 size);

/**
 * @brief TODO
 *
 * @param size TODO
 * @return TODO
 */
void *calloc(u32 size);

/**
 * @brief TODO
 *
 * @param p TODO
 * @param size TODO
 * @return TODO
 */
void *realloc(void *p, u32 size);

#endif /* __ALLOC_H__ */
