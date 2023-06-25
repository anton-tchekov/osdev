/**
 * @file    disk.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    07.06.2023
 */

#include <disk.h>
#include <syscall.h>

/* TODO: ADD SYSCALLS */

u32 disk_count(void)
{
	return 0;
}

u32 disk_size(u32 id)
{
	return 0;
	(void)id;
}

Status disk_read(u32 id, u32 block, void *buf)
{
	return STATUS_NOT_IMPLEMENTED;
	(void)id, (void)block, (void)buf;
}

Status disk_write(u32 id, u32 block, const void *buf)
{
	return STATUS_NOT_IMPLEMENTED;
	(void)id, (void)block, (void)buf;
}
