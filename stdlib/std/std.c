#include <std.h>

void exit(i32 status)
{
	syscall1(SYSCALL_EXIT, status);
}
