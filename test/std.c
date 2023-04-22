#include "std.h"
#include "syscall.h"



void print(char *s)
{
	syscall1(0, (u32)s);
}

void exit(void)
{
	syscall0(1);
}

int strlen(const char *s)
{
	int i;
	for(i = 0; *s++; ++i)
	{
	}

	return i;
}