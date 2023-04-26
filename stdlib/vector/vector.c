/**
 * @file    vector.c
 * @author  Tim Gabrikowski, Anton YTchekov
 * @version 0.1
 * @date    26.04.2023
 */

#include "vector.h"

/* TODO: Check parameters of all functions with ASSERT */

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

void vector_replace(
	Vector *vector, u32 index, u32 count, void *elems, i32 new_count)
{
	/* TODO: Implementation */

	/* 1. Step */
	/* Calculate new number of elements */

	/* (2. Step) if new_length > vector->Capacity */
	/* Resize the vector by allocating a new buffer with malloc and copying
		all the elements using memcpy */

	/* (3. Step) if inserting in the middle or beginning */
	/* Shift the end of the array up to make space for new elements or
		down to reduce number of elements depending on if count <> new_count */

	/* (4. Step) if new_count > 0 */
	/* Copy new elements into range (index, new_count) using memcpy */

	/* Would be nice to combine steps 2, 3, 4 so that there is no
	unnesseccary copying of data that will be overwritten anyway */
}

void *vector_get(Vector *vector, u32 index)
{
	return vector->Data + vector->ElementSize * index;
}
