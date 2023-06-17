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

/** The size of each memory bank in bytes */
#define XMEM_CHIP_SIZE                 0x20000UL

/** Chip address mask */
#define XMEM_CHIP_MASK                 0x1FFFFUL

/* --- MEMORY --- */

/**
 * @brief Read data from physical memory
 *
 * @param chip Which memory chip
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void env_memory_read(u8 chip, u32 addr, void *data, u32 size);

/**
 * @brief Write data to physical memory
 *
 * @param chip Which memory chip
 * @param addr Start address
 * @param data Data buffer
 * @param size Size in bytes
 */
void env_memory_write(u8 chip, u32 addr, const void *data, u32 size);

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
void env_gfx_rect(u16 x, u16 y, u16 w, u16 h, u32 color);

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
