#include "types.h"
#include "syscall.h"

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


void main(void)
{
	print(reverse("hello world\n"));
	exit();
}
