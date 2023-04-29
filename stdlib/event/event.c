#include <event.h>
#include <syscall.h>
#include <keyboard.h>

void finish(void)
{
	syscall0(SYSCALL_FINISH);
}

static void _event_loop_handler(void)
{
	loop();
	finish();
}

void __setup(void) __attribute__((section(".start")));
void __setup(void)
{
	event_register(EVENT_LOOP, _event_loop_handler);
	keyboard_init();
	setup();
	finish();
}

void event_register(Event type, void *event)
{
	syscall2(SYSCALL_EVENT_REGISTER, type, (u32)event);
}
