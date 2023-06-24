#include "mouse.h"
#include "types.h"
#include "isr.h"
#include "io.h"
#include "graphics.h"
#include "cursor.c"

static u8 mouse_cycle = 0;
static i8 mouse_byte[3];

#define MOUSE_PORT          0x60
#define MOUSE_STATUS        0x64
#define MOUSE_BIT_A         0x02
#define MOUSE_BIT_B         0x01
#define MOUSE_TIMEOUT       100000

#define MOUSE_CLICK_LEFT    0x01
#define MOUSE_CLICK_RIGHT   0x02
#define MOUSE_CLICK_MIDDLE  0x04

typedef struct
{
	i32 X, Y, W, H;
	i32 State;
} Button;

#define BTN_CNT 5

Button btns[BTN_CNT] =
{
	{ .X = 400, .Y = 400, .W = 50, .H = 50, .State = 0 },
	{ .X = 470, .Y = 400, .W = 50, .H = 50, .State = 0 },
	{ .X = 540, .Y = 400, .W = 50, .H = 50, .State = 0 },
	{ .X = 610, .Y = 400, .W = 50, .H = 50, .State = 0 },
	{ .X = 680, .Y = 400, .W = 50, .H = 50, .State = 0 },
};



static bool button_pressed(Button *btn, i32 x, i32 y)
{
	return x >= btn->X && y >= btn->Y &&
		x < btn->X + btn->W && y < btn->Y + btn->H;
}

static void mouse_event(i32 dx, i32 dy, u32 buttons)
{
	static i32 mx = 500, my = 500;

	if(buttons & MOUSE_CLICK_LEFT)
	{
		for(int i = 0; i < BTN_CNT; ++i)
		{
			if(button_pressed(btns + i, mx, my))
			{
				graphics_rect(btns[i].X, btns[i].Y, btns[i].W, btns[i].H,
					graphics_color(0xFF, 0x00, 0x00));
			}
		}
	}

	graphics_restore(mx, my, CURSOR_ARROW_WIDTH, CURSOR_ARROW_HEIGHT);

	mx += dx;
	my -= dy;

	mx = i32_constrain(mx, 0, graphics_width() - CURSOR_ARROW_WIDTH);
	my = i32_constrain(my, 0, graphics_height() - CURSOR_ARROW_HEIGHT);

	graphics_cursor(mx, my, CURSOR_ARROW_WIDTH, CURSOR_ARROW_HEIGHT,
		_cursor_arrow);
}

static void mouse_wait_a_bit(void)
{
	u32 timeout = MOUSE_TIMEOUT;
	while(--timeout)
	{
		if(((inb(MOUSE_STATUS) & MOUSE_BIT_A)) == 0)
		{
			return;
		}
	}
}

static void mouse_wait_b_bit(void)
{
	u32 timeout = MOUSE_TIMEOUT;
	while(--timeout)
	{
		if((inb(MOUSE_STATUS) & MOUSE_BIT_B) == 1)
		{
			return;
		}
	}
}

static void mouse_write(u8 write)
{
	mouse_wait_a_bit();
	outb(MOUSE_STATUS, 0xD4);
	mouse_wait_a_bit();
	outb(MOUSE_PORT, write);
}

static u8 mouse_read(void)
{
	mouse_wait_b_bit();
	return inb(MOUSE_PORT);
}

static void mouse_handler(registers_t r)
{
	u8 status = inb(MOUSE_STATUS);
	while(status & MOUSE_BIT_B)
	{
		i8 mouse_in = inb(MOUSE_PORT);
		if(status & 0x20)
		{
			switch(mouse_cycle)
			{
				case 0:
					mouse_byte[0] = mouse_in;
					if(!(mouse_in & 0x08))
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
	mouse_wait_a_bit();
	outb(MOUSE_STATUS, 0xA8);
	mouse_wait_a_bit();
	outb(MOUSE_STATUS, 0x20);
	mouse_wait_b_bit();
	status = inb(0x60) | 0x02;
	mouse_wait_a_bit();
	outb(MOUSE_STATUS, 0x60);
	mouse_wait_a_bit();
	outb(MOUSE_PORT, status);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	isr_register(IRQ12, mouse_handler);


	for(int i = 0; i < BTN_CNT; ++i)
	{
		graphics_rect(btns[i].X, btns[i].Y, btns[i].W, btns[i].H,
			graphics_color(0x00, 0xFF, 0x00));
	}
}
