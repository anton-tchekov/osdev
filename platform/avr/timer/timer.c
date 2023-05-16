/**
 * @file    timer.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 */

#include <timer.h>
#include <logger.h>
#include <avr/pgmspace.h>

static const char _msg_timer[] PROGMEM = "Millisecond timer initialized";

void timer_init(void)
{
	/* TODO: Initialize hardware timers */
	log_boot(_msg_timer);
}

u32 millis(void)
{
	/* TODO: Implement millis function */
	/* Return number of ms since boot */
	return 0;
}
