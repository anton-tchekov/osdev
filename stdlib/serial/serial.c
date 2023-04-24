/**
 * @file    serial.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <serial.h>
#include <syscall.h>

i32 serial_write(const void *buf, i32 n)
{
	return syscall2(SYSCALL_SERIAL_WRITE, (u32)buf, n);
}

i32 serial_read(const void *buf, i32 n)
{
	return syscall2(SYSCALL_SERIAL_READ, (u32)buf, n);
}
