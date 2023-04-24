/**
 * @file    algo.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    25.04.2023
 */

#include <algo.h>

void qsort(void *base, u32 number, u32 width,
	i32 (*compare)(const void *, const void *))
{

}

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *))
{
	const char *base0 = (const char *)base;
	const void *p;
	i32 lim, cmp;

	for(lim = nitems; lim; lim >>= 1)
	{
		p = base0 + (lim >> 1) * size;
		cmp = (*compar)(key, p);
		if(!cmp)
		{
			return (void *)p;
		}
		else if(cmp > 0)
		{
			base0 = (const char *)p + size;
			--lim;
		}
	}

	return NULL;
}
