#ifndef __VSNPRINTF_H__
#define __VSNPRINTF_H__

#include "types.h"
#include <stdarg.h>

i32 vsnprintf(char *str, u32 size, const char *format, va_list arg);

#endif /* __VSNPRINTF_H__ */