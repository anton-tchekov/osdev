/**
 * @file    platform.h
 * @author  Anton Tchekov
 * @version 0.3
 * @date    16.05.2023
 * @brief   Interface between Emulator and Drivers.
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
u32 env_memory_size(void);

/**
 * @brief Read data from physical memory
 *
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void env_memory_read(u32 addr, void *data, u32 size);

/**
 * @brief Write data to physical memory
 *
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void env_memory_write(u32 addr, const void *data, u32 size);

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
void env_gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color);

/**
 * @brief Draw an RGBA image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u32 addr);

/**
 * @brief Draw an RGB image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u32 addr);

/**
 * @brief Draw an RGB565 image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u32 addr);

/**
 * @brief Draw a colored grayscale image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 * @param fg ABGR foreground color
 * @param bg ABGR background color
 */
void env_gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, u32 addr, u32 fg, u32 bg);

/**
 * @brief Draw a colored 1-bit image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 * @param fg ABGR foreground color
 * @param bg ABGR background color
 */
void env_gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, u32 addr, u32 fg, u32 bg);

/* --- SERIAL --- */

/**
 * @brief Send data over the serial interface
 *
 * @param data The data to send
 * @param len The number of bytes
 */
void env_serial_write(const void *data, u32 len);

/* --- RANDOM --- */

/**
 * @brief Generate a pseudo-random number.
 *
 * @return The number
 */
u32 env_random_get(void);

/* --- TIMER --- */

/**
 * @brief Get the number of milliseconds since boot
 *
 * @return The number of milliseconds
 */
u32 env_millis(void);

#endif /* __PLATFORM_H__ */
