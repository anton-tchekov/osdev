/**
 * @file    time.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 */

#include <time.h>
#include <syscall.h>

u32 millis(void)
{
	return syscall0(SYSCALL_MILLIS);
}
