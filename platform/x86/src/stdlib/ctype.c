#include "ctype.h"
#include "strchr.h"

bool isalnum(i32 c)
{
	return isalpha(c) || isdigit(c);
}

bool isalpha(i32 c)
{
	return islower(c) || isupper(c);
}

bool iscntrl(i32 c)
{
	return !isprint(c);
}

bool isdigit(i32 c)
{
	return c >= '0' && c <= '9';
}

bool isgraph(i32 c)
{
	return c >= 33 && c <= 126;
}

bool islower(i32 c)
{
	return c >= 'a' && c <= 'z';
}

bool isprint(i32 c)
{
	return c >= 32 && c <= 126;
}

bool ispunct(i32 c)
{
	return strchr("!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~", c) != NULL;
}

bool isspace(i32 c)
{
	return strchr(WHITESPACE, c) != NULL;
}

bool isupper(i32 c)
{
	return c >= 'A' && c <= 'Z';
}

bool isxdigit(i32 c)
{
	return isdigit(c) ||
		   (c >= 'A' && c <= 'F') ||
		   (c >= 'a' && c <= 'f');
}

i32 tolower(i32 c)
{
	return isupper(c) ? c - 'A' + 'a' : c;
}

i32 toupper(i32 c)
{
	return islower(c) ? c - 'a' + 'A' : c;
}
