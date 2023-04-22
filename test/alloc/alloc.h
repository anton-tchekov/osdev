#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "types.h"

#define HEAP_INIT_SIZE 0x10000
#define HEAP_MAX_SIZE 0xF0000
#define HEAP_MIN_SIZE 0x10000

#define MIN_ALLOC_SZ 4

#define MIN_WILDERNESS 0x2000
#define MAX_WILDERNESS 0x1000000

#define BIN_COUNT 9
#define BIN_MAX_IDX (BIN_COUNT - 1)

typedef unsigned int uint;

typedef struct node_t {
    uint hole;
    uint size;
    struct node_t* next;
    struct node_t* prev;
} node_t;

typedef struct { 
    node_t *header;
} footer_t;

typedef struct {
    node_t* head;
} bin_t;

typedef struct {
    long start;
    long end;
    bin_t *bins[BIN_COUNT];
} heap_t;


void free(heap_t *heap, void *p);

void *alloc(heap_t *heap, size_t size);

#endif /* __ALLOC_H__ */