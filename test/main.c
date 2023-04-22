#include "types.h"
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

char *reverse(char *s)
{
	int i, j, tmp;
	for(i = 0, j = strlen(s) - 1; i < j; ++i, --j)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	return s;
}

void main(void) __attribute__((section(".start")));

void main(void)
{
	print(reverse("hello world\n"));
	exit();
}
