/**
 * @file    alloc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    08.05.2023
 */

#include <alloc.h>

#define ALIGN        4
#define MIN_ALLOC    8
#define END          0xFFFFFFFF

typedef struct CHUNK_HEADER
{
	u32 Size, Next;
} ChunkHeader;

static ptr _heap_start;
static ChunkHeader _first;
static MemAllocInfo _info;

/* --- PRIVATE --- */
static inline void *_chunk_data(ChunkHeader *header)
{
	return (void *)((ptr)header + sizeof(ChunkHeader));
}

static inline ChunkHeader *_chunk_header(void *p)
{
	return (ChunkHeader *)((ptr)p - sizeof(ChunkHeader));
}

static inline ChunkHeader *_get_next(u32 next)
{
	if(next == END)
	{
		return NULL;
	}

	return (ChunkHeader *)(_heap_start + next);
}

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
	if (size == 0)
	{
		return NULL;
	}

	/* Minimum allocation size */
	if (size < MIN_ALLOC)
	{
		size = MIN_ALLOC;
	}

	/* Ensure alignment */
	size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

	_info.Used += size;
	_info.Total += size + sizeof(ChunkHeader);

	/* Find free chunk */
	for (prev = &_first, cur = _get_next(prev->Next);
		 cur; prev = cur, cur = _get_next(cur->Next))
	{
		if (size + sizeof(ChunkHeader) == cur->Size)
		{
			/* --- Perfect fit --- */
			/* Remove chunk from free list */
			prev->Next = cur->Next;

			/* Return pointer to data area */
			return _chunk_data(cur);
		}
		else if (size + 2 * sizeof(ChunkHeader) < cur->Size)
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

	for (prev = &_first, next = _get_next(prev->Next);
		 next; prev = next, next = _get_next(next->Next))
	{
		if (next > header)
		{
			/* Merge chunks if possible */
			bool mergePrev = ((ptr)prev + prev->Size == (ptr)header);
			bool mergeNext = ((ptr)header + header->Size == (ptr)next);

			if (mergeNext && mergePrev)
			{
				/* Both Prev and Next are free */
				prev->Next = next->Next;
				prev->Size += header->Size + next->Size;
			}
			else if (mergePrev)
			{
				/* Prev is free */
				prev->Size += header->Size;
			}
			else if (mergeNext)
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

#ifdef TEST

#include <stdio.h>

void print_chain(void)
{
	i32 i = 0;
	ChunkHeader *cur;
	printf("\n| Block | Adr Dec  | Size Dec |\n");
	for (cur = _get_next(_first.Next), i = 1;
		 cur; cur = _get_next(cur->Next), i++)
	{
		u32 offset = (ptr)cur - (ptr)_heap_start;
		printf("| #%04d | %8d | %8d |\n",
			i, offset, cur->Size);
	}
}

void print_stats(void)
{
	f32 percent_used = (f32)_used / (f32)_size * 100.0f;
	i32 free = _size - _total;
	f32 percent_free = (f32)free / (f32)_size * 100.0f;
	f32 percent_total = (f32)_total / (f32)_size * 100.0f;
	i32 wasted = _total - _used;
	f32 percent_wasted = (f32)wasted / (f32)_total * 100.0f;
	f32 wu_ratio = (f32)_used / (f32)wasted;
	printf(
		"header size                  = %d bytes\n"
		"heap size                    = %6d (%6.2f%%)\n"
		"total stored (used + header) = %6d (%6.2f%%)\n"
		"data stored (used)           = %6d (%6.2f%%)\n"
		"wasted (header)              = %6d (%6.2f%%)\n"
		"free                         = %6d (%6.2f%%)\n"
		"used to waste factor         = 1 : %3.2f\n",
		(u32)sizeof(ChunkHeader),
		_size, 100.0f,
		_total, percent_total,
		_used, percent_used,
		wasted, percent_wasted,
		free, percent_free,
		wu_ratio);
}

#endif /* TEST */
