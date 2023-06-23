/**
 * @file    types.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Shorthand type definitions inspired by Rust
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** Signed 64-bit integer */
typedef int64_t i64;

/** Signed 32-bit integer */
typedef int32_t i32;

/** Signed 16-bit integer */
typedef int16_t i16;

/** Signed 8-bit integer */
typedef int8_t i8;

/** Unsigned 64-bit integer */
typedef uint64_t u64;

/** Unsigned 32-bit integer */
typedef uint32_t u32;

/** Unsigned 16-bit integer */
typedef uint16_t u16;

/** Unsigned 8-bit integer */
typedef uint8_t u8;

/** 32-bit float */
typedef float f32;

/** 64-bit float */
typedef double f64;

/** Pointer size integer */
typedef uintptr_t ptr;

static inline i32 i32_min(i32 a, i32 b)
{
	return (a < b) ? a : b;
}

static inline i32 i32_max(i32 a, i32 b)
{
	return (a > b) ? a : b;
}

static inline u32 u32_min(u32 a, u32 b)
{
	return (a < b) ? a : b;
}

static inline u32 u32_max(u32 a, u32 b)
{
	return (a > b) ? a : b;
}

static inline i32 i32_constrain(i32 val, i32 min, i32 max)
{
	return i32_max(i32_min(val, max), min);
}

static inline u32 u32_constrain(u32 val, u32 min, u32 max)
{
	return u32_max(u32_min(val, max), min);
}

#endif /* __TYPES_H__ */
