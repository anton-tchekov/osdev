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
 * @brief Get the size of the installed memory
 *
 * @return The size of the memory in bytes
 */
u32 memory_size(void);

/**
 * @brief Read data from physical memory
 *
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void memory_read(u32 addr, void *data, u32 size);

/**
 * @brief Write data to physical memory
 *
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void memory_write(u32 addr, const void *data, u32 size);

/* --- GRAPHICS --- */

/**
 * @brief Draw rectangle
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param color Fill color
 */
void gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color);

u32 syscall_gfx_image_rgba(u32 *args);
u32 syscall_gfx_image_rgb(u32 *args);
u32 syscall_gfx_image_rgb565(u32 *args);
u32 syscall_gfx_image_grayscale(u32 *args);
u32 syscall_gfx_image_1bit(u32 *args);

/* --- SERIAL --- */

/**
 * @brief Send data over the serial interface
 *
 * @param data The data to send
 * @param len The number of bytes
 */
void serial_write(const void *data, u32 len);

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
