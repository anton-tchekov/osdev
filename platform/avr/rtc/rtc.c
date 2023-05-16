/**
 * @file    rtc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <rtc.h>
#include <logger.h>
#include <avr/pgmspace.h>

static const char _msg_rtc[] PROGMEM = "RTC initialized";

void rtc_init(void)
{
	/* TODO: Initialize RTC */
	log_boot(_msg_rtc);
}

DateTime *datetime_now(DateTime *now)
{
	/* TODO: Get Date and Time from RTC */
	now->Year = 1970;
	now->Month = 1;
	now->Day = 1;

	now->Hour = 0;
	now->Minute = 0;
	now->Second = 0;

	return now;
}
