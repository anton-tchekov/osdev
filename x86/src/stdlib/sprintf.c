#include "sprintf.h"
#include "vsnprintf.h"

i32 sprintf(char *str, const char *format, ...)
{
	i32 r;
	va_list args;
	va_start(args, format);
	r = vsnprintf(str, UINT32_MAX, format, args);
	va_end(args);
	return r;
}

i32 snprintf(char *str, u32 size, const char *format, ...)
{
	i32 r;
	va_list args;
	va_start(args, format);
	r = vsnprintf(str, size, format, args);
	va_end(args);
	return r;
}
