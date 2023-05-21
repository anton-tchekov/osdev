/**
 * @file    event.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    29.04.2023
 */

#include <event.h>
#include <syscall.h>
#include <keyboard.h>
#include <alloc.h>

void finish(void)
{
	syscall0(SYSCALL_FINISH);
}

/** Wrapper function for main loop */
static void _event_loop_handler(void)
{
	loop();
	finish();
}

void __setup(void) __attribute__((section(".start")));

/** Wrapper function for setup function */
void __setup(void)
{
	event_register(EVENT_LOOP, (u32)_event_loop_handler);

	/* TODO: What are good values? */
	memalloc_init(0x10000, 0x40000);

	keyboard_init();
	setup();
	finish();
}

void event_register(Event type, u32 event)
{
	syscall2(SYSCALL_EVENT_REGISTER, type, event);
}
