#ifndef __BSEARCH_H__
#define __BSEARCH_H__

#include "types.h"

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *));

#endif /* __BSEARCH_H__ */
