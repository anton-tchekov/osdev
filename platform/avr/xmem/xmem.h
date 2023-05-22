/**
 * @file    xmem.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   External memory driver (23LC1024)
 */

#ifndef __XMEM_H__
#define __XMEM_H__

#include <types.h>

/**
 * @brief Initialize external memory and run memory test
 */
void xmem_init(void);

/**
 * @brief Read the specified number of bytes from the external memory
 *
 * @param addr Start address
 * @param data Destination buffer
 * @param size Number of bytes to read
 */
void xmem_read(u32 addr, void *data, u16 size);

/**
 * @brief Write the specified number of bytes to the external memory
 *
 * @param addr Start address
 * @param data Source buffer
 * @param size Number of bytes to write
 */
void xmem_write(u32 addr, const void *data, u16 size);

/**
 * @brief Set a specified number of bytes in the external memory to a value
 *
 * @param addr Start address
 * @param value The value to write
 * @param size Number of bytes to set
 */
void xmem_set(u32 addr, u8 value, u16 size);

/**
 * @brief Get external memory size
 *
 * @return Size of the external memory in bytes
 */
u32 xmem_size(void);

#endif /* __XMEM_H__ */
