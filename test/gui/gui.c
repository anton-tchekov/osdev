#include <gui.h>

static void element_first(void)
{
	u32 i;
	for(i = 0; i < current_form->Count; ++i)
	{
		if(current_form->Elements[i].Type == ELEMENT_TYPE_BUTTON ||
			current_form->Elements[i].Type == ELEMENT_TYPE_INPUT)
		{
			current_element = i;
			break;
		}
	}
}

static void element_next(void)
{
	u32 i;
	for(i = current_element + 1; i < current_form->Count; ++i)
	{
		if(current_form->Elements[i].Type == ELEMENT_TYPE_BUTTON)
		{
			element_render_sel(&current_form->Elements[current_element], 0);
			element_render_sel(&current_form->Elements[i], 1);
			current_element = i;
			break;
		}

		if(current_form->Elements[i].Type == ELEMENT_TYPE_INPUT)
		{
			element_render_sel(&current_form->Elements[current_element], 0);
			element_render_sel(&current_form->Elements[i], 1);
			current_element = i;
			break;
		}
	}
}

static void element_prev(void)
{
	u32 i;
	for(i = current_element - 1; i >= 0; --i)
	{
		if(current_form->Elements[i].Type == ELEMENT_TYPE_BUTTON)
		{
			element_render_sel(&current_form->Elements[current_element], 0);
			element_render_sel(&current_form->Elements[i], 1);
			current_element = i;
			break;
		}

		if(current_form->Elements[i].Type == ELEMENT_TYPE_INPUT)
		{
			element_render_sel(&current_form->Elements[current_element], 0);
			element_render_sel(&current_form->Elements[i], 1);
			current_element = i;
			break;
		}
	}
}

void window_open(Window *window)
{
	current_form = window;
	current_element = -1;
	element_first();
	window_render(window);
}

static void window_render(Window *f)
{
	u32 i;

	/* Title Bar */
	lcd_rect(0, 0, LCD_WIDTH, 20, LCD_BLUE);

	/* Background */
	lcd_rect(0, 20, LCD_WIDTH, 220, LCD_WHITE);

	/* Title */
	lcd_string(4, 4, f->Title, LCD_WHITE);

	for(i = 0; i < f->Count; ++i)
	{
		element_render(&f->Elements[i]);
	}
}

static void element_render_sel(Element *e, u32 sel)
{
	switch(e->Type)
	{
		case ELEMENT_TYPE_LABEL:
			label_render(&e->E.L);
			break;

		case ELEMENT_TYPE_BUTTON:
			button_render(&e->E.B, sel);
			break;

		case ELEMENT_TYPE_INPUT:
			input_render(&e->E.I, sel);
			break;
	}
}

static void element_render(Element *e)
{
	u32 sel = ((current_element >= 0) && &current_form->Elements[current_element] == e);
	switch(e->Type)
	{
		case ELEMENT_TYPE_LABEL:
			label_render(&e->E.L);
			break;

		case ELEMENT_TYPE_BUTTON:
			button_render(&e->E.B, sel);
			break;

		case ELEMENT_TYPE_INPUT:
			input_render(&e->E.I, sel);
			break;
	}
}

static void label_render(Label *l)
{
	lcd_string(l->X, 20 + l->Y, l->Text.RAM, LCD_BLACK);
}

static void button_render(Button *b, u32 sel)
{
	int16_t x, y;
	y = 20 + b->Y;

	lcd_rect(b->X, y, b->W, b->H, LCD_WHITE);

	if(sel)
	{
		lcd_rect_border2(b->X, y, b->W, b->H, LCD_RED);
	}
	else
	{
		lcd_rect_border(b->X, y, b->W, b->H, LCD_BLACK);
	}

	x = b->X + b->W / 2 - lcd_string_width(b->Text) / 2;
	lcd_string(x, y, b->Text);
}

static void input_render(Input *i, u32 sel)
{
	lcd_rect(i->X + 1, 20 + i->Y + 1, i->W - 2, 20 - 2, LCD_WHITE);
	if(sel)
	{
		lcd_rect_border2(i->X, 20 + i->Y, i->W, 20, LCD_RED);
	}
	else
	{
		lcd_rect_border(i->X, 20 + i->Y, i->W, 20, LCD_BLACK);
	}

	if(sel)
	{
		lcd_vline(i->X + 5 + lcd_string_width_len(i->Text, i->Position) - 1, 20 + i->Y + 3, 14, LCD_BLACK);
	}

	lcd_string(i->X + 5, 20 + i->Y + 5, i->Text, LCD_BLACK);
}

static void input_grow(Input *i, u32 n)
{
	if(i->Text[i->Position])
	{
		u32 j;
		for(j = i->Length - 1; j >= i->Position; --j)
		{
			i->Text[j + n] = i->Text[j];
		}
	}

	i->Length += n;
	i->Text[i->Length] = '\0';
}

static void input_shrink(Input *i, u32 n)
{
	if(i->Text[i->Position])
	{
		u32 j = i->Position;
		for(; i->Text[j]; ++j)
		{
			i->Text[j - n] = i->Text[j];
		}
	}
}

static void input_insert(Input *i, char c)
{
	if(i->Length + 1 < i->Size)
	{
		input_grow(i, 1);
		i->Text [i->Position++] = c;
		input_render(i, 1);
	}
}

static void input_clear(Input *i)
{
	i->Length = 0;
	i->Position = 0;
	i->Text[0] = '\0';
	input_render(i, 1);
}

static void input_delete(Input *i)
{
	if(i->Position > 0)
	{
		input_shrink(i, 1);
		--(i->Position);
		--(i->Length);
		i->Text[i->Length] = '\0';
		input_render(i, 1);
	}
}

static void input_left(Input *i)
{
	if(i->Position > 0)
	{
		--(i->Position);
		input_render(i, 1);
	}
}

static void input_right(Input *i)
{
	if(i->Position < i->Length)
	{
		++(i->Position);
		input_render(i, 1);
	}
}

static void input_event_key(Input *i, char c)
{
	if(ascii >= 32 && ascii <= 126)
	{
		input_insert(i, (char)ascii);
	}
	else if(ascii == '\b')
	{
		input_delete(i);
	}
}

static void form_event_button(u32 button, u32 action)
{
	Element *ce;
	if(!current_form)
	{
		return;
	}

	if(current_form->OnKey)
	{
		current_form->OnKey(button, action);
	}

	if(current_element < 0)
	{
		return;
	}

	ce = &current_form->Elements[current_element];
	if(action == BUTTON_RELEASE)
	{
		if(ce->Type == ELEMENT_TYPE_BUTTON)
		{

		}

		return;
	}

	if(ce->Type == ELEMENT_TYPE_BUTTON ||
		ce->Type == ELEMENT_TYPE_INPUT)
	{
		if(button == BUTTON_UP)
		{
			element_prev();
		}
		else if(button == BUTTON_DOWN)
		{
			element_next();
		}
		else
		{
			if(ce->Type == ELEMENT_TYPE_BUTTON)
			{
				if(button == BUTTON_CENTER)
				{
					ce->E.B.Click();
				}
			}
			else if(ce->Type == ELEMENT_TYPE_INPUT)
			{
				if(button == BUTTON_LEFT)
				{
					input_left(&ce->E.I);
				}
				else if(button == BUTTON_RIGHT)
				{
					input_right(&ce->E.I);
				}
			}
		}
	}
}

static void form_event_key(u32 key, u32 ascii)
{
	Element *ce;
	if(!current_form)
	{
		return;
	}

	if(current_element < 0)
	{
		return;
	}

	ce = &current_form->Elements[current_element];
	if(ce->Type == ELEMENT_TYPE_INPUT)
	{
		input_event_key(&ce->E.I, key, ascii);
	}
}