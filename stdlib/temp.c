
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

Button button = BUTTON_CREATE(10, 10, 100, 30, "Click Me!", NULL);

void *elements[] =
{
	&button,
};


	/*reverse(str);
	serial_write(str, strlen(str));
	window_open(&window);*/



Window window =
{
	.Title = "Test",
	.Elements = elements,
	.Count = ARRLEN(elements),
	.OnKey = NULL
};
