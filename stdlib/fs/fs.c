/**
 * @file    fs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <fs.h>

Status file_open(const char *name, File *file)
{
	return syscall2(SYSCALL_FILE_OPEN, (u32)name, (u32)file);
}

Status file_read(File file, u32 start, u32 len, void *buf)
{
	return syscall4(SYSCALL_FILE_READ, file, start, len, (u32)buf);
}

Status file_write(File file, u32 start, u32 len, const void *buf)
{
	return syscall4(SYSCALL_FILE_WRITE, file, start, len, (u32)buf);
}

Status file_close(File file)
{
	return syscall1(SYSCALL_FILE_CLOSE, file);
}

Status file_size(File file, u32 *size)
{
	return syscall2(SYSCALL_FILE_SIZE, file, (u32)size);
}