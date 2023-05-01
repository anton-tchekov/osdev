/**
 * @file    serial.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <serial.h>
#include <syscall.h>
#include <stdarg.h>
#include <format.h>

void debug_print(const char *format, ...)
{
	char str[128];
	int r;
	va_list args;
	va_start(args, format);
	r = vsnprintf(str, UINT32_MAX, format, args);
	va_end(args);

	serial_write(str, r);
}

i32 serial_write(const void *buf, i32 n)
{
	return syscall2(SYSCALL_SERIAL_WRITE, (u32)buf, n);
}
