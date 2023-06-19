#include "mouse.h"
#include "types.h"
#include "isr.h"
#include "io.h"
#include "graphics.h"

static u8 mouse_cycle = 0;
static i8 mouse_byte[3];

#define MOUSE_IRQ          12

#define MOUSE_PORT          0x60
#define MOUSE_STATUS        0x64
#define MOUSE_ABIT          0x02
#define MOUSE_BBIT          0x01
#define MOUSE_WRITE         0xD4
#define MOUSE_F_BIT         0x20
#define MOUSE_V_BIT         0x08

#define MOUSE_CLICK_LEFT    0x01
#define MOUSE_CLICK_RIGHT   0x02
#define MOUSE_CLICK_MIDDLE  0x04

static void mouse_event(i32 dx, i32 dy, u32 buttons)
{
	static i32 mx = 500, my = 500;

graphics_rect(10, 10, 10, 10,graphics_color(255, 255, 255));

	mx += dx;
	my += dy;

	if(mx <= 0)
	{
		mx = 0;
	}

	if(my <= 0)
	{
		my = 0;
	}

	graphics_rect(mx, my, 10, 10, graphics_color(255, 255, 255));
}

static i32 mouse_wait(u8 a_type)
{
	u32 timeout = 100000;
	if(!a_type)
	{
		while(--timeout)
		{
			if((inb(MOUSE_STATUS) & MOUSE_BBIT) == 1)
			{
				return 0;
			}
		}
	}
	else
	{
		while(--timeout)
		{
			if (!((inb(MOUSE_STATUS) & MOUSE_ABIT)))
			{
				return 0;
			}
		}
	}

	return 1;
}

static void mouse_write(u8 write)
{
	mouse_wait(1);
	outb(MOUSE_STATUS, MOUSE_WRITE);
	mouse_wait(1);
	outb(MOUSE_PORT, write);
}

static u8 mouse_read(void)
{
	mouse_wait(0);
	return inb(MOUSE_PORT);
}

static void mouse_handler(registers_t r)
{
	u8 status = inb(MOUSE_STATUS);
	while(status & MOUSE_BBIT)
	{
		i8 mouse_in = inb(MOUSE_PORT);
		if(status & MOUSE_F_BIT)
		{
			switch(mouse_cycle)
			{
				case 0:
					mouse_byte[0] = mouse_in;
					if(!(mouse_in & MOUSE_V_BIT))
					{
						return;
					}

					++mouse_cycle;
					break;

				case 1:
					mouse_byte[1] = mouse_in;
					++mouse_cycle;
					break;

				case 2:
					mouse_byte[2] = mouse_in;
					if(mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40)
					{
						break;
					}

					mouse_event(mouse_byte[1], mouse_byte[2], mouse_byte[0]);
					mouse_cycle = 0;
					break;
			}
		}

		status = inb(MOUSE_STATUS);
	}

	(void)r;
}

void mouse_init(void)
{
	u8 status;
	mouse_wait(1);
	outb(MOUSE_STATUS, 0xA8);
	mouse_wait(1);
	outb(MOUSE_STATUS, 0x20);
	mouse_wait(0);
	status = inb(0x60) | 2;
	mouse_wait(1);
	outb(MOUSE_STATUS, 0x60);
	mouse_wait(1);
	outb(MOUSE_PORT, status);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	isr_register(MOUSE_IRQ, mouse_handler);
}
