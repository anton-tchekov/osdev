/**
 * @file    vector.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    26.04.2023
 */

#include <vector.h>
#include <alloc.h>
#include <string.h>
#include <panic.h>

/**
 * @brief Next power of two
 *
 * @param n The number for which to get the next bigger power of two for
 * @return The power of two, for example for input 26 it would return 32
 */
static u32 _next_pot(u32 n)
{
	u32 power = 1;
	while(power < n)
	{
		power <<= 1;
	}

	return power;
}

void vector_init(Vector *vector, u32 element_size, u32 initial_capacity)
{
	vector->ElementSize = element_size;
	vector->Capacity = initial_capacity;
	vector->Length = 0;
	vector->Data = memalloc(element_size * initial_capacity);
}

void vector_destroy(Vector *vector)
{
	memfree(vector->Data);
}

void vector_replace(
	Vector *vector, u32 index, u32 count, const void *elems, u32 new_count)
{
	u32 new_length, element_size;
	element_size = vector->ElementSize;

	ASSERT(index + count <= vector->Length, "invalid range");

	/* Calculate new number of elements */
	new_length = vector->Length - count + new_count;
	if(new_length > vector->Capacity)
	{
		/* Resize necessary */
		u32 new_capacity, prev_bytes, new_bytes, old_bytes, last_bytes;
		void *new_data;

		new_capacity = _next_pot(new_length);
		prev_bytes = index * element_size;
		new_bytes = new_count * element_size;
		old_bytes = count * element_size;
		last_bytes = (vector->Length - index - count) * element_size;

		/* Calculate new capacity */
		vector->Capacity = new_capacity;

		/* Create new buffer */
		new_data = memalloc(new_capacity * element_size);

		/* Copy first part from previous buffer */
		memcpy(new_data, vector->Data, prev_bytes); // correct

		/* Copy new range */
		memcpy((u8 *)new_data + prev_bytes, elems, new_bytes); // correct

		/* Copy last part */
		memcpy((u8 *)new_data + prev_bytes + new_bytes,
			(u8 *)vector->Data + prev_bytes + old_bytes,
			last_bytes);

		/* Replace with new buffer */
		memfree(vector->Data);
		vector->Data = new_data;
	}
	else
	{
		u32 prev_bytes, new_bytes, old_bytes, last_bytes;

		prev_bytes = index * element_size;
		new_bytes = new_count * element_size;
		old_bytes = count * element_size;
		last_bytes = (vector->Length - index - count) * element_size;

		/* Shift last part */
		memmove((u8 *)vector->Data + prev_bytes + new_bytes,
			(u8 *)vector->Data + prev_bytes + old_bytes,
			last_bytes);

		/* Copy new range */
		memcpy((u8 *)vector->Data + prev_bytes, elems, new_bytes);
	}

	vector->Length = new_length;
}
