/**
 * @file    util.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    07.06.2023
 */

#include <util.h>

void write_32(u8 *buf, u32 val)
{
	buf[0] = val & 0xFF;
	buf[1] = (val >> 8) & 0xFF;
	buf[2] = (val >> 16) & 0xFF;
	buf[3] = (val >> 24) & 0xFF;
}

u32 read_32(u8 *buf)
{
	/* The cast is important for systems with small integer size */
	return (u32)(buf[0]) |
		((u32)(buf[1]) << 8) |
		((u32)(buf[2]) << 16) |
		((u32)(buf[3]) << 24);
}

void write_16(u8 *buf, u16 v)
{
	buf[0] = v & 0xFF;
	buf[1] = (v >> 8) & 0xFF;
}

u16 read_16(u8 *buf)
{
	return (u16)(buf[0]) | ((u16)(buf[1]) << 8);
}
