#include <std.h>
#include <gui.h>

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

void main(void)
{
	print(reverse("hello world\n"));
	exit(0);
}
