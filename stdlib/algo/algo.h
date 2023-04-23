#ifndef __ALGO_H__
#define __ALGO_H__

#include <types.h>

void qsort(void *base, u32 number, u32 width,
	i32 (*compare)(const void *, const void *));

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compar)(const void *, const void *));

#endif /* __ALGO_H__ */
