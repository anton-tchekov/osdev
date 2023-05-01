/**
 * @file    xmem.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   External memory (23LC1024)
 */

#ifndef __XMEM_H__
#define __XMEM_H__

#include <types.h>

void xmem_read(u32 addr, void *data, u16 size);
void xmem_write(u32 addr, const void *data, u16 size);
void xmem_set(u32 addr, u8 value, u16 size);
void xmem_cpy(u32 dest, u32 src, u32 count);
i8 memory_cmp(u8 bank, u16 a, u16 b, u16 count);
u16 memory_chr(u8 bank, u16 addr, u8 value, u16 count);

#endif /* __XMEM_H__ */
