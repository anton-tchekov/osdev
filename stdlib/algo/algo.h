/**
 * @file    algo.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    25.04.2023
 * @brief   TODO
 */

#ifndef __ALGO_H__
#define __ALGO_H__

#include <types.h>

/**
 * @brief TODO
 *
 * @param base TODO
 * @param number TODO
 * @param width TODO
 * @param compare TODO
 */
void qsort(void *base, u32 number, u32 width,
	i32 (*compare)(const void *, const void *));

/**
 * @brief TODO
 *
 * @param key TODO
 * @param base TODO
 * @param nitems TODO
 * @param size TODO
 * @param compare TODO
 */
void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *));

#endif /* __ALGO_H__ */
