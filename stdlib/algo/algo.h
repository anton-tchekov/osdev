/**
 * @file    algo.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    25.04.2023
 * @brief   Library for common algorithms
 */

#ifndef __ALGO_H__
#define __ALGO_H__

#include <types.h>

/**
 * @brief Binary search
 *
 * @param key Key to be found
 * @param base Pointer to a sorted array with `nitems`
 * @param nitems Number of items
 * @param size Size of each item in bytes
 * @param compare Comparison function that receives `key` and current item
 * @return Pointer to found item or NULL
 */
void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *));

#endif /* __ALGO_H__ */
