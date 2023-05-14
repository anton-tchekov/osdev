/**
 * @file    alloc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    08.05.2023
 */

#include <alloc.h>

#define ALIGN        sizeof(ptr)
#define MIN_ALLOC    8

typedef struct CHUNK_HEADER
{
	u32 Size;
	struct CHUNK_HEADER *Next;
} ChunkHeader;

static u32 _used, _total, _size;
static ptr _heap_start;
static ChunkHeader _first;

/* --- PRIVATE --- */
static void *_chunk_data(ChunkHeader *header)
{
	return (void *)((ptr)header + sizeof(ChunkHeader));
}

static ChunkHeader *_chunk_header(void *p)
{
	return (ChunkHeader *)((ptr)p - sizeof(ChunkHeader));
}

/* --- PUBLIC --- */
void memalloc_init(ptr heap_start, i32 size)
{
	_first.Next = (ChunkHeader *)heap_start;
	_first.Next->Size = size;
	_first.Next->Next = NULL;
	_heap_start = heap_start;
	_size = size;
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

	_used += size;
	_total += size + sizeof(ChunkHeader);

	/* Find free chunk */
	for (prev = &_first, cur = prev->Next;
		 cur; prev = cur, cur = cur->Next)
	{
		if (size + sizeof(ChunkHeader) == cur->Size)
		{
			/* --- Perfect fit --- */
			/* Remove chunk from free list */
			prev->Next = cur->Next;
			cur->Next = NULL;

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
			prev->Next = second;

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

	_used -= header->Size - sizeof(ChunkHeader);
	_total -= header->Size;

	for (prev = &_first, next = prev->Next;
		 next; prev = next, next = next->Next)
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
				prev->Next = header;
			}
			else
			{
				/* Both sides are used */
				header->Next = prev->Next;
				prev->Next = header;
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
	for (cur = _first.Next, i = 1;
		 cur; cur = cur->Next, i++)
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
		"heap size                    = %6d (%6.2f%%)\n"
		"total stored (used + header) = %6d (%6.2f%%)\n"
		"data stored (used)           = %6d (%6.2f%%)\n"
		"wasted (header)              = %6d (%6.2f%%)\n"
		"free                         = %6d (%6.2f%%)\n"
		"used to waste factor         = 1 : %3.2f\n",
		_size, 100.0f,
		_total, percent_total,
		_used, percent_used,
		wasted, percent_wasted,
		free, percent_free,
		wu_ratio);
}

#endif /* TEST */
