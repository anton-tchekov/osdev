#ifndef __UTF8_H__
#define __UTF8_H__

#include "types.h"

i32 codepoint_utf8(i32 codepoint, char *out);
const char *utf8_codepoint(const char *s, i32 *out);
u32 utf8_length(const char *s);

#endif /* __UTF8_H__ */
