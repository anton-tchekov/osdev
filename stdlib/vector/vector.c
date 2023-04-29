/**
 * @file    vector.c
 * @author  Tim Gabrikowski, Anton YTchekov
 * @version 0.1
 * @date    26.04.2023
 */

#include "vector.h"

void vector_init(Vector *vector, u32 element_size, u32 initial_capacity)
{
	vector->ElementSize = element_size;
	vector->Capacity = initial_capacity;
	vector->Length = 0;
	vector->Data = malloc(element_size * initial_capacity);
}

void vector_destroy(Vector *vector)
{
	free(vector->Data);
}

static u32 _next_pot(u32 n)
{
	u32 power = 1;
	while(power < n)
	{
		power <<= 1;
	}

	return power;
}

void vector_replace(
	Vector *vector, u32 index, u32 count, void *elems, u32 new_count)
{
	u32 element_size = vector->ElementSize;

	ASSERT(index <= vector.Length);
	ASSERT(count <= vector.Length);
	ASSERT((index + count) <= vector.Length);

	/* Calculate new number of elements */
	new_length = vector->Length - count + new_count;
	if(new_length > vector->Capacity)
	{
		/* Resize necessary */
		u32 first_bytes;
		u32 new_capacity;
		void *new_data;

		new_capacity = _next_pot(new_length);

		first_bytes = index * element_size;

		/* Create new buffer */
		new_data = malloc(new_capacity * element_size);

		/* Copy first part */
		memcpy(new_data, vector->Data, first_bytes);

		/* Copy new range */
		memcpy(new_data + first_bytes,
			vector->Data, new_count * element_size);

		/* Copy last part */
		memcpy(new_data + first_bytes,
			vector->Data, new_count * element_size);

		/* Replace with new buffer */
		free(vector->Data);
		vector->Data = new_data;
	}
	else
	{
		/* Shift last part */
		memmove(vector->Data + ,
			vector->Data + ,
			(vector->Length - index - count) * element_size)

		/* Copy new range */
		memcpy(vector->Data + index * vector->ElementSize,
			elems, new_count * vector->ElementSize);
	}
}

void *vector_get(Vector *vector, u32 index)
{
	return vector->Data + vector->ElementSize * index;
}
