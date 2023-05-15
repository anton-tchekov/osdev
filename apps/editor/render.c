
static void _render_char(i32 x, i32 y, char c, Color fg, Color bg)
{

}

static void _render_line(Line *line, i32 line_number)
{
	/* syntax highlighting */
	static const char *_keywords[] =
	{
		"var",
		"return",
		"continue",
		"break",
		"if",
		"elif",
		"else",
		"for",
		"to",
		"loop",
		"do",
		"while",
		"switch",
		"case",
		"default"
	};

	char *p;
	i32 i, len;

	p = line->Buffer;
	len = line->Length;
	for(i = 0; i < len; ++i)
	{
		c = p[i];
		if(c == '/')
		{
			if(i + i < len && p[i + 1] == '/')
			{
				for(; i < len; ++i)
				{
					c = p[i];
					_render_char(x, y, c, COLOR_GREEN, COLOR_BLACK);
				}
			}
		}
		else if(c == '\"')
		{
			/* string start */
			while(c != '\"')
			{
				/* skip escape sequence */
				if(c == '\\')
				{
					++i;
				}

				_render_char(x, y, c, COLOR_RED, COLOR_BLACK);
			}
		}
		else if(c == '{' || c == '}' ||
			c == '[' || c == ']' ||
			c == '(' || c == ')')
		{
			_render_char(x, y, c, COLOR_YELLOW, COLOR_BLACK);
		}
		else if(is_identifer_start_char(c))
		{
			i32 id_start;

			id_start = i;
			while(is_identifer_char(c))
			{
			}

			if(is_identifier())
			{

			}
		}
	}
}
