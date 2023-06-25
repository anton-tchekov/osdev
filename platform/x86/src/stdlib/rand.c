#include "rand.h"

static u32 _x;

void srand(u32 seed)
{
	_x = seed;
}

u32 rand(void)
{
	register u32 x = _x;
	x ^= x << 13;
	x ^= x << 17;
	x ^= x << 5;
	_x = x;
	return x;
}
