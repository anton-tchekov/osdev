/**
 * @file    rand.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <rand.h>
#include <syscall.h>

i32 rand(void)
{
	return syscall0(SYSCALL_RAND);
}
