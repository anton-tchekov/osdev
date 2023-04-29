/**
 * @file    time.c
 * @author  Tim Gabrikowski, Anton YTchekov
 * @version 0.1
 * @date    26.04.2023
 */

#include <time.h>
#include <syscall.h>

void datetime_now(DateTime *now)
{
	syscall1(SYSCALL_DATETIME_NOW, (u32)now);
}

u32 millis(void)
{
	return syscall0(SYSCALL_MILLIS);
}
