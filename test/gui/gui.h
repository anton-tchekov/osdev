
typedef struct LABEL
{
	uint8_t Flags;
	int16_t X, Y;
	char *Text;
} Label;

typedef struct BUTTON
{
	uint8_t Flags;
	int16_t X, Y, W, H;
	char *Text;
	void (*Click)(void);
} Button;

typedef struct INPUT
{
	int16_t X, Y, W;
	uint8_t Flags, Position, Length, Size;
	char *Text;
} Input;

typedef struct ELEMENT
{
	uint8_t Type;
	union
	{
		Label *L;
		Input *I;
		Button *B;
	} E;
} Element;

typedef struct
{
	u8 Flags;
	char *Title;
	Element *Elements;
	i32 Count;
	void (*OnKey)(uint8_t, uint8_t);
} Window;

static Form *current_form;
static int8_t current_element;

static void form_open(Form *form);
static void form_render(Form *f);
static void element_render_sel(Element *e, uint8_t sel);
static void element_render(Element *e);
static void label_render(Label *l);
static void button_render(Button *b, uint8_t sel);
static void input_render(Input *i, uint8_t sel);
static void form_event_button(uint8_t button, uint8_t action);
static void form_event_key(uint8_t key, uint8_t ascii);
static void input_grow(Input *i, uint8_t n);
static void input_shrink(Input *i, uint8_t n);

