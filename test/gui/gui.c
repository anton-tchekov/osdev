#define ELEMENT_TYPE_LABEL  0
#define ELEMENT_TYPE_BUTTON 1
#define ELEMENT_TYPE_INPUT  2

#define FLAG_TEXT_PROGMEM   1


static void element_first(void)
{
	int8_t i;
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
	int8_t i;
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
	int8_t i;
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

static void form_open(Form *form)
{
	current_form = form;
	current_element = -1;
	element_first();
	form_render(form);
}

static void form_render(Form *f)
{
	uint8_t i;

	/* Title Bar */
	lcd_color(LCD_BLUE);
	lcd_rect(0, 0, LCD_WIDTH, 20);

	/* Background */
	lcd_color(LCD_WHITE);
	lcd_rect(0, 20, LCD_WIDTH, 220);

	/* Title */
	if(f->Flags & FLAG_TEXT_PROGMEM)
	{
		lcd_string_P(4, 4, f->Title.Flash);
	}
	else
	{
		lcd_string(4, 4, f->Title.RAM);
	}

	for(i = 0; i < f->Count; ++i)
	{
		element_render(f->Elements + i);
	}
}

static void element_render_sel(Element *e, uint8_t sel)
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
	uint8_t sel = ((current_element >= 0) && &current_form->Elements[current_element] == e);
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
	lcd_color(LCD_BLACK);
	if(l->Flags & FLAG_TEXT_PROGMEM)
	{
		lcd_string_P(l->X, 20 + l->Y, l->Text.Flash);
	}
	else
	{
		lcd_string(l->X, 20 + l->Y, l->Text.RAM);
	}
}

static void button_render(Button *b, uint8_t sel)
{
	int16_t x, y;
	y = 20 + b->Y;

	lcd_color(LCD_WHITE);
	lcd_rect(b->X, y, b->W, b->H);

	if(sel)
	{
		lcd_color(LCD_RED);
		lcd_rect_border2(b->X, y, b->W, b->H);
	}
	else
	{
		lcd_color(LCD_BLACK);
		lcd_rect_border(b->X, y, b->W, b->H);
	}

	lcd_color(LCD_BLACK);
	if(b->Flags & FLAG_TEXT_PROGMEM)
	{
		x = b->X + b->W / 2 - lcd_string_width_P(b->Text.Flash) / 2;
		lcd_string_P(x, y + b->H / 2 - 5, b->Text.Flash);
	}
	else
	{
		x = b->X + b->W / 2 - lcd_string_width(b->Text.RAM) / 2;
		lcd_string(x, y, b->Text.RAM);
	}
}

static void input_render(Input *i, uint8_t sel)
{
	lcd_color(LCD_WHITE);
	lcd_rect(i->X + 1, 20 + i->Y + 1, i->W - 2, 20 - 2);

	if(sel)
	{
		lcd_color(LCD_RED);
		lcd_rect_border2(i->X, 20 + i->Y, i->W, 20);
	}
	else
	{
		lcd_color(LCD_BLACK);
		lcd_rect_border(i->X, 20 + i->Y, i->W, 20);
	}

	lcd_color(LCD_BLACK);
	if(sel)
	{
		lcd_vline(i->X + 5 + lcd_string_width_len(i->Text, i->Position) - 1, 20 + i->Y + 3, 14);
	}

	lcd_string(i->X + 5, 20 + i->Y + 5, i->Text);
}

static void input_grow(Input *i, uint8_t n)
{
	if(i->Text[i->Position])
	{
		int8_t j;
		for(j = i->Length - 1; j >= i->Position; --j)
		{
			i->Text[j + n] = i->Text[j];
		}
	}

	i->Length += n;
	i->Text[i->Length] = '\0';
}

static void input_shrink(Input *i, uint8_t n)
{
	if(i->Text[i->Position])
	{
		int8_t j = i->Position;
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

static void input_event_key(Input *i, uint8_t key, uint8_t ascii)
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

static void form_event_button(uint8_t button, uint8_t action)
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

static void form_event_key(uint8_t key, uint8_t ascii)
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