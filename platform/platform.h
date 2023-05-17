/**
 * @file    platform.h
 * @author  Anton Tchekov
 * @version 0.3
 * @date    16.05.2023
 * @brief   Interface between Emulator and Drivers
 *          Arguably the most important file in this project
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <types.h>
#include <keyboard-shared.h>

/* --- MEMORY --- */

/**
 * @brief
 *
 * @param addr
 * @param data
 * @param size
 */
void memory_read(u32 addr, void *data, u32 size);

/**
 * @brief
 *
 * @param addr
 * @param data
 * @param size
 */
void memory_write(u32 addr, const void *data, u32 size);

/* --- SCHEDULING - TO BE UPDATED/REMOVED --- */

/**
 * @brief
 */
void process_setup(void);

/**
 * @brief
 */
void os_update(void);

u32 syscall_exit(u32 *args);

/* --- GRAPHICS --- */

u32 syscall_gfx_rect(u32 *args);

/* --- NOT SURE ABOUT THESE --- */

u32 syscall_gfx_image_rgba(u32 *args);
u32 syscall_gfx_image_rgb(u32 *args);
u32 syscall_gfx_image_rgb565(u32 *args);
u32 syscall_gfx_image_grayscale(u32 *args);
u32 syscall_gfx_image_1bit(u32 *args);

/* --- KEYBOARD --- */

/**
 * @brief
 *
 * @param key
 * @param chr
 * @param down
 */
void keyboard_event(Key key, i32 chr, KeyState down);

/* --- SERIAL --- */

u32 syscall_serial_write(u32 *args);

/* --- RANDOM --- */

/**
 * @brief Generate a pseudo-random number.
 *
 * @return The number
 */
u32 random_get(void);

/* --- RTC --- */

/**
 * @brief Get the current date and time
 *
 * @param now Output parameter
 */
DateTime *datetime_now(DateTime *now);

/* --- TIMER --- */

/**
 * @brief Get the number of milliseconds since boot
 *
 * @return The number of milliseconds
 */
u32 millis(void);

#endif /* __PLATFORM_H__ */
