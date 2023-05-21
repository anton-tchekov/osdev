/**
 * @file    rtc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <rtc.h>
#include <logger.h>
#include <avr/pgmspace.h>

void rtc_init(void)
{
	log_boot_P(LOG_INIT, PSTR("RTC driver initialized"));
}

DateTime *datetime_now(DateTime *now)
{
	now->Year = 1970;
	now->Month = 1;
	now->Day = 1;

	now->Hour = 0;
	now->Minute = 0;
	now->Second = 0;

	return now;
}
