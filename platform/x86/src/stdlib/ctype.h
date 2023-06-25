#ifndef __CTYPE_H__
#define __CTYPE_H__

#include "types.h"

#define WHITESPACE " \n\t\v\f\r"

bool isalnum(i32 c);
bool isalpha(i32 c);
bool iscntrl(i32 c);
bool isdigit(i32 c);
bool isgraph(i32 c);
bool islower(i32 c);
bool isprint(i32 c);
bool ispunct(i32 c);
bool isspace(i32 c);
bool isupper(i32 c);
bool isxdigit(i32 c);
i32 tolower(i32 c);
i32 toupper(i32 c);

#endif /* __CTYPE_H__ */
