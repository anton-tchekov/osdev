#include <rand.h>
#include <syscall.h>

i32 rand(void)
{
	return syscall0(SYSCALL_RAND);
}
