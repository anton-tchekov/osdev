#ifndef __SPRINTF_H__
#define __SPRINTF_H__

#include "types.h"
#include <stdarg.h>

i32 sprintf(char *str, const char *format, ...);
i32 snprintf(char *str, u32 size, const char *format, ...);

#endif /* __SPRINTF_H__ */
