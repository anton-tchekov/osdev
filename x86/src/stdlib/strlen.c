#include "strlen.h"

u32 strlen(const char *str)
{
	const char *s = str;
	while(*s) { ++s; }
	return s - str;
}

u32 strnlen(const char *s, u32 max)
{
	const char *s = str;
	while(max-- && *s) { ++s; }
	return s - str;
}
