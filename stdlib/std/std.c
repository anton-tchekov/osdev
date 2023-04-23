#include <std.h>
#include <syscall.h>

void exit(i32 status)
{
	syscall1(SYSCALL_EXIT, status);
}

i32 main(void);

void start(void) __attribute__((section(".start")));
void start(void)
{
	exit(main());
}
