/**
 * @file    alloc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    08.05.2023
 */

#include <alloc.h>

/** Allocation alignment */
#define ALIGN        4

/** Minimum allocation size in bytes */
#define MIN_ALLOC    8

/** Heap end */
#define END          0xFFFFFFFF

/** Linked list chunk header */
typedef struct CHUNK_HEADER
{
	/** Allocation size in bytes */
	u32 Size;

	/** Next block offset */
	u32 Next;
} ChunkHeader;

/** Heap start address */
static ptr _heap_start;

/** First dummy memory chunk */
static ChunkHeader _first;

/** Memory allocator state info */
static MemAllocInfo _info;

/* --- PRIVATE --- */

/**
 * @brief Get a pointer to the data region of a memory chunk
 *
 * @param header Chunk header
 * @return Pointer to data area
 */
static inline void *_chunk_data(ChunkHeader *header)
{
	return (void *)((ptr)header + sizeof(ChunkHeader));
}

/**
 * @brief Get the chunk header for a pointer that is about to be freed
 *
 * @param p Pointer
 * @return The corresponding header
 */
static inline ChunkHeader *_chunk_header(void *p)
{
	return (ChunkHeader *)((ptr)p - sizeof(ChunkHeader));
}

/**
 * @brief Get the next chunk header in the linked list
 *
 * @param next Chunk offset
 * @return Pointer to next chunk header or NULL if it is the last one
 */
static inline ChunkHeader *_get_next(u32 next)
{
	if(next == END)
	{
		return NULL;
	}

	return (ChunkHeader *)(_heap_start + next);
}

/**
 * @brief Get to offset of a chunk header on the heap
 *
 * @param header Pointer to chunk header
 * @return Offset in bytes after the start of the heap
 */
static inline u32 _get_offset(ChunkHeader *header)
{
	return (ptr)header - _heap_start;
}

/* --- PUBLIC --- */
void memalloc_init(ptr heap_start, i32 size)
{
	ChunkHeader *next;

	_heap_start = heap_start;
	_info.Size = size;

	_first.Next = 0;

	next = _get_next(_first.Next);
	next->Size = size;
	next->Next = END;
}

void *memalloc(u32 size)
{
	ChunkHeader *cur, *prev;
	if(size == 0)
	{
		return NULL;
	}

	/* Minimum allocation size */
	if(size < MIN_ALLOC)
	{
		size = MIN_ALLOC;
	}

	/* Ensure alignment */
	size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

	_info.Used += size;
	_info.Total += size + sizeof(ChunkHeader);

	/* Find free chunk */
	for(prev = &_first, cur = _get_next(prev->Next);
		cur; prev = cur, cur = _get_next(cur->Next))
	{
		if(size + sizeof(ChunkHeader) == cur->Size)
		{
			/* --- Perfect fit --- */
			/* Remove chunk from free list */
			prev->Next = cur->Next;

			/* Return pointer to data area */
			return _chunk_data(cur);
		}
		else if(size + 2 * sizeof(ChunkHeader) < cur->Size)
		{
			/* --- First fit --- */
			/* Split chunk in two */
			ChunkHeader *second =
				(ChunkHeader *)((ptr)cur + sizeof(ChunkHeader) + size);

			second->Size = cur->Size - sizeof(ChunkHeader) - size;
			second->Next = cur->Next;
			prev->Next = _get_offset(second);

			/* Reduce the size of first part by the size of the second part */
			cur->Size = size + sizeof(ChunkHeader);

			/* Return pointer to data area of second part */
			return _chunk_data(cur);
		}
	}

	/* Very unlikely */
	return NULL;
}

void memfree(void *p)
{
	ChunkHeader *next, *prev, *header;

	header = _chunk_header(p);

	_info.Used -= header->Size - sizeof(ChunkHeader);
	_info.Total -= header->Size;

	for(prev = &_first, next = _get_next(prev->Next);
		next; prev = next, next = _get_next(next->Next))
	{
		if(next > header)
		{
			/* Merge chunks if possible */
			bool mergePrev = ((ptr)prev + prev->Size == (ptr)header);
			bool mergeNext = ((ptr)header + header->Size == (ptr)next);

			if(mergeNext && mergePrev)
			{
				/* Both Prev and Next are free */
				prev->Next = next->Next;
				prev->Size += header->Size + next->Size;
			}
			else if(mergePrev)
			{
				/* Prev is free */
				prev->Size += header->Size;
			}
			else if(mergeNext)
			{
				/* Next is free */
				header->Size += next->Size;
				header->Next = next->Next;
				prev->Next = _get_offset(header);
			}
			else
			{
				/* Both sides are used */
				header->Next = prev->Next;
				prev->Next = _get_offset(header);
			}
			return;
		}
	}
}
