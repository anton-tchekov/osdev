/* TODO: Documentation */

#include <timer.h>

timer_init(void)
{
	/* TODO */
}

u32 millis(void)
{
	/* TODO */
	return 0;
}

void datetime_now(DateTime *now)
{
	now->Year = 1970;
	now->Month = 1;
	now->Day = 1;

	now->Hour = 0;
	now->Minute = 0;
	now->Second = 0;
}
