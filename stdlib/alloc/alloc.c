/**
 * @file    alloc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    08.05.2023
 */

#include <alloc.h>

#define ALIGN sizeof(ptr)
#define MIN_ALLOC 8

typedef struct CHUNK_HEADER
{
	u32 Size;
	struct CHUNK_HEADER *Next;
} ChunkHeader;

static u32 _used, _total, _size;

static ChunkHeader _first;

static ChunkHeader *_free_list = &_first;

static ptr _heap_start;

static void *_chunk_data(ChunkHeader *header)
{
	return (void *)((ptr)header + sizeof(ChunkHeader));
}

static ChunkHeader *_chunk_header(void *p)
{
	return (ChunkHeader *)((ptr)p - sizeof(ChunkHeader));
}

void memfree(void *p)
{
	ChunkHeader *cur, *prev, *header;

	header = _chunk_header(p);

	_used -= header->Size - sizeof(ChunkHeader);
	_total -= header->Size;

	for (prev = _free_list, cur = prev->Next;
		 cur; prev = cur, cur = cur->Next)
	{
		if (cur > header)
		{
			/* Merge chunks */
			bool mergePrev = ((ptr)prev + prev->Size == (ptr)header);
			bool mergeNext = ((ptr)header + header->Size == (ptr)cur);

			if (mergeNext && mergePrev)
			{
				prev->Next = cur->Next;
				prev->Size += header->Size + cur->Size;
			}
			else if (mergePrev)
			{
				/* Prev and Freed are Adjacent */
				prev->Size += header->Size;
			}
			else if (mergeNext)
			{
				/* Freed and Cur are Adjacent */
				header->Size += cur->Size;
				header->Next = cur->Next;
				prev->Next = header;
			}
			else
			{
				/* Update Links */
				header->Next = prev->Next;
				prev->Next = header;
			}
			return;
		}
	}
}

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
		if (size == cur->Size - sizeof(ChunkHeader))
		{
			/* --- Perfect fit --- */
			/* Remove chunk from free list */
			prev->Next = cur->Next;
			cur->Next = NULL;

			/* Return pointer to data area */
			return _chunk_data(cur);
		}
		else if (size < cur->Size - 2 * sizeof(ChunkHeader))
		{
			/* --- First fit --- */
			/* Split chunk in two */
			ChunkHeader *second = (ChunkHeader *)((ptr)cur + size + sizeof(ChunkHeader));

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

#ifdef TEST

#include <stdio.h>

void print_chain(void)
{
	i32 i = 0;
	ChunkHeader *cur;
	printf("| Block | Adr Hex    | Adr Dec  | Size Hex | Size Dec |\n");
	for (cur = _first.Next, i = 1;
		 cur; cur = cur->Next, i++)
	{
		ptr offset = (ptr)cur - (ptr)_heap_start;
		printf("| #%04d | 0x%08X | %08d | 0x%06X | %08d |\n", i, offset, offset, cur->Size, cur->Size);
	}
}

void print_stats(void)
{
	f32 percent_used = ((f32)_used / (f32)_size) * 100;
	f32 percent_free = (((f32)_size - (f32)_total) / _size) * 100;
	f32 wasted = ((f32)_total - (f32)_used);
	f32 percent_wasted = (wasted / (f32)_total) * 100;
	f32 wu_ratio = (f32)_used / wasted;
	printf(
		"data stored(used) = %d\n"
		"total stored (used + header) = %d\n"
		"free = %f\n"
		"total available = %d\n"
		"percent used of all = %f\n"
		"percent wasted of all = %f\n"
		"used per waste factor = x%f\n",
		_used, _total, percent_free, _size, percent_used, percent_wasted, wu_ratio);
}

#endif /* TEST */
