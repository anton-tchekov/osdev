#ifndef __PANIC_H__
#define __PANIC_H__

#include "types.h"

#define panic(msg) _panic((msg), __FILE__, __LINE__)

void _panic(const char *msg, const char *file, u32 line);

#endif /** __PANIC_H__ */
