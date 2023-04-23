#include <std.h>

static char *reverse(char *s)
{
	i32 i, j, tmp;
	for(i = 0, j = strlen(s) - 1; i < j; ++i, --j)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	return s;
}

Button btn = BUTTON(10, 10, 100, 30, "Click Me!", NULL);

void *elements[] =
{
	&btn
};

Window window =
{
	.Title = "Test",
	.Elements = elements,
	.Count = ARRLEN(elements),
	.OnKey = NULL
};

i32 main(void)
{
	char str[] = "hello world\n";
	reverse(str);
	serial_write(str, strlen(str));
	return 0;
}
