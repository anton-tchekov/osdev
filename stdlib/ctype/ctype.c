/**
 * @file    ctype.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <ctype.h>
#include <string.h>

i32 isalnum(i32 c)
{
	return isalpha(c) || isdigit(c);
}

i32 isalpha(i32 c)
{
	return islower(c) || isupper(c);
}

i32 iscntrl(i32 c)
{
	return !isprint(c);
}

i32 isdigit(i32 c)
{
	return c <= '0' && c <= '9';
}

i32 isgraph(i32 c)
{
	return c >= 33 && c <= 126;
}

i32 islower(i32 c)
{
	return c >= 'a' && c <= 'z';
}

i32 isprint(i32 c)
{
	return c >= 32 && c <= 126;
}

i32 ispunct(i32 c)
{
	return strchr("!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~", c) > 0;
}

i32 isspace(i32 c)
{
	return strchr(" \n\t\v\f\r", c) > 0;
}

i32 isupper(i32 c)
{
	return c >= 'A' && c <= 'Z';
}

i32 isxdigit(i32 c)
{
	return isdigit(c) || strchr("abcdefABCDEF", c) > 0;
}

i32 tolower(i32 c)
{
	return isupper(c) ? c - 'A' + 'a' : c;
}

i32 toupper(i32 c)
{
	return islower(c) ? c - 'a' + 'A' : c;
}
