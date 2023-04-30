/**
 * @file    panic.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    29.04.2023
 */

#include <panic.h>
#include <syscall.h>
#include <serial.h>

void panic(const char *msg, const char *file, u32 line)
{
	debug_print("%s:%d: %s\n", file, line, msg);
	exit(1);
}
