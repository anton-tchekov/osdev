/**
 * @file    alloc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    08.05.2023
 */

#include <alloc.h>

#define HEAP_START_ADDR   0x100000
#define HEAP_SIZE         0x100000
#define ALIGN                    4
#define MIN_ALLOC                8

typedef struct CHUNK_HEADER
{
	u32 Size;
	struct CHUNK_HEADER *Next;
} ChunkHeader;

static ChunkHeader _first =
{
	.Size = 0,
	.Next = (ChunkHeader *)HEAP_START_ADDR
};

static ChunkHeader *_free_list = &_first;

static void _chunk_remove(ChunkHeader *prev, ChunkHeader *cur)
{
	prev->Next = cur->Next;
}

static void _chunk_insert(ChunkHeader *prev, ChunkHeader *item)
{
	item->Next = prev->Next;
	prev->Next = item;
}

static void *_chunk_data(ChunkHeader *header)
{
	return (void *)((ptr)header + sizeof(ChunkHeader));
}

void free(void *p)
{
	ChunkHeader *cur, *prev, *header;

	header = p - sizeof(ChunkHeader);

	for(prev = _free_list, cur = prev->Next;
		cur; prev = cur, cur = cur->Next)
	{
		if(cur > header)
		{
			/* TODO: Merge chunks */
			if(prev + prev->Size == header)
			{
				/* Prev and Freed are Adjacent */
			}

			if(header + header->Size == cur)
			{
				/* Freed and Cur are Adjacent */
			}

			/* Update Links */
			_chunk_insert(prev, header);
		}
	}
}

void alloc_init(void)
{
	_free_list->Size = HEAP_SIZE;
	_free_list->Next = NULL;
}

void *malloc(u32 size)
{
	ChunkHeader *cur, *prev;

	/* Minimum allocation size */
	if(size < MIN_ALLOC)
	{
		size = MIN_ALLOC;
	}

	/* Ensure alignment */
	size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

	/* Find free chunk */
	for(prev = _free_list, cur = prev->Next;
		cur; prev = cur, cur = cur->Next)
	{
		if(size == cur->Size - sizeof(ChunkHeader))
		{
			/* --- Perfect fit --- */
			/* Remove chunk from free list */
			_chunk_remove(prev, cur);
			cur->Next = NULL;

			/* Return pointer to data area */
			return _chunk_data(cur);
		}
		else if(size < cur->Size - 2 * sizeof(ChunkHeader))
		{
			/* --- First fit --- */
			/* Split chunk in two */
			ChunkHeader *second = (ChunkHeader *)
					((ptr)cur + cur->Size - size - sizeof(ChunkHeader));

			second->Size = sizeof(ChunkHeader) + size;
			second->Next = NULL;

			/* Reduce the size of first part by the size of the second part */
			cur->Size -= size + sizeof(ChunkHeader);

			/* Return pointer to data area of second part */
			return _chunk_data(second);
		}
	}

	/* Very unlikely */
	return NULL;
}
