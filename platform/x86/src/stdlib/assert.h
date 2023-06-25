#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "panic.h"

#define assert(cond) \
	do \
	{ \
		if(!(cond)) { \
			panic("Assertion failed: " #cond); \
		} \
	} while(0)

#endif /* __ASSERT_H__ */
