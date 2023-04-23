#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <types.h>

void free(void *p);
void *malloc(size_t size);
void *calloc(size_t size);
void *realloc(void *p, size_t size);

#endif /* __ALLOC_H__ */
