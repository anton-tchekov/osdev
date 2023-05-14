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

/** DateTime struct containing date and time info */
typedef struct
{
	/** Year, for example 2023 */
	i32 Year;

	/** Month, value from 1-12 */
	i32 Month;

	/** Day, value from 1-31 */
	i32 Day;

	/** Hour, value from 0-23 */
	i32 Hour;

	/** Minute, value from 0-59 */
	i32 Minute;

	/** Second, value from 0-59 */
	i32 Second;
} DateTime;

/** Rectangle structure consisting of X, Y, Width and Height */
typedef struct
{
	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Width */
	i32 W;

	/** Height */
	i32 H;
} Rectangle;

/** Key State: Released (Key Up), Pressed (Key Down), Repeat (Held Down) */
typedef enum
{
	KEYSTATE_RELEASED,
	KEYSTATE_PRESSED,
	KEYSTATE_REPEAT
} KeyState;

/** Keycode */
typedef u32 Key;

/** Keyboard Modifiers */
#define MOD_SHIFT  (Key)(1 << 31)
#define MOD_CTRL   (Key)(1 << 30)
#define MOD_OS     (Key)(1 << 29)
#define MOD_ALT    (Key)(1 << 28)
#define MOD_ALT_GR (Key)(1 << 27)

const char *keystate_string(KeyState state);

#endif /* __TYPES_H__ */
