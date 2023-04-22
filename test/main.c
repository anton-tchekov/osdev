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

Button btn = {
	.X = 10,
	.Y = 10,
	.W = 100,
	.H = 30,
	.Text = "",
};

void main(void)
{
	print(reverse("hello world\n"));
	exit(0);
}
