/**
 * @file    vector.h
 * @author  Tim Gabrikowski, Anton YTchekov
 * @version 0.1
 * @date    26.04.2023
 * @brief   Generic resizable array
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "types.h"

/** Vector type */
typedef struct
{
	/** The size of each element in bytes */
	u32 ElementSize;

	/** Current number of elements */
	u32 Length;

	/** Current maximum number of elements */
	u32 Capacity;

	/** Buffer containing elements */
	void *Data;
} Vector;

/**
 * @brief Initialize a vector with an element size and an initial capacity
 *
 * Example code for a vector of integers:
 * @code
 * Vector vector;
 * vector_init(&vector, sizeof(i32), 16);
 * @endcode
 *
 * @param vector The vector
 * @param element_size The size of each element in the vector
 * @param initial_capacity The vectors initial capacity
 */
void vector_init(Vector *vector, u32 element_size, u32 initial_capacity);

/**
 * @brief Frees the internal memory buffer of the vector
 *
 * @param vector The vector
 */
void vector_destroy(Vector *vector);

/**
 * @brief Replace a range of elements in the vector with new elements
 *
 * @param items The new elements
 * @param index
 */
void vector_replace(Vector *vector, void *items, u32 index, u32 count);

/**
 * @brief Get a pointer to an element in the vector
 *
 * @param v The Vector
 * @param index The index
 * @return Pointer to the element
 */
void *vector_get(Vector *vector, u32 index);


static inline void vector_insert_range(Vector *vector, u32 index, u32 count)
{

}

/**
 * @brief Insert a single element
 *
 *
 */
static inline void vector_insert(Vector *vector, u32 index, u32 count)
{

}

/**
 * @brief Remove a range of elements
 *
 * @param index Start index of the range
 * @param count Number of elements to remove
 */
static inline void vector_remove_range(Vector *vector, u32 index, u32 count)
{
	vector_insert(vector, NULL, index, count);
}

/**
 * @brief Remove a single element
 *
 * @param index Index of the element to remove
 */
static inline void vector_remove(Vector *vector, u32 index)
{
	vector_remove_range(vector, index, 1);
}

/* --- STACK FUNCTIONS --- */

/**
 * @brief Insert an element after the last element of the vector
 *
 * @param elem The element to insert
 */
static inline void vector_push(Vector *vector, void *elem)
{

}

/**
 * @brief Get the last element from the stack and remove it afterwards.
 *
 * @param elem The element to insert
 */
static inline void vector_pop(Vector *vector, void *item)
{

}

static inline void vector_peek(Vector *vector, void *item)
{

}

#endif /* __VECTOR_H__ */
