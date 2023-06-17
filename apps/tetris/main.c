/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 * @brief   Tetris game for the OS
 */

#include <std.h>

/** Field width in blocks */
#define FIELD_WIDTH         10

/** Field height in blocks */
#define FIELD_HEIGHT        20

/** Field size in blocks */
#define FIELD_SIZE            (FIELD_WIDTH * FIELD_HEIGHT)

/** Size of each block in pixels */
#define BLOCK_SIZE          20

/** Default falling speed in milliseconds */
#define FALL_SPEED_DEFAULT 200

/** Falling speed when down button is pressed */
#define FALL_SPEED_FAST     30

/** Game state: running */
#define STATUS_RUNNING       1

/** Piece Type */
typedef enum { I, J, L, O, S, T, Z } PieceType;

/** A moving tetris piece */
typedef struct
{
	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Piece rotation */
	i32 Rotation;

	/** Piece Type, see enum above */
	PieceType Type;
} Piece;

/** Struct containing a description of a tetris piece */
typedef struct
{
	/** 4x4 bitmap for 4 rotation states */
	u16 Blocks[4];

	/** Color of the tetromino */
	Color Fill;
} PieceData;

/** All 7 tetrominos */
const PieceData Pieces[] =
{
	{
		/* I */
		{ 0x0F00, 0x2222, 0x00F0, 0x4444 },
		COLOR_CYAN
	},
	{
		/* J */
		{ 0x44C0, 0x8E00, 0x6440, 0x0E20 },
		COLOR_BLUE
	},
	{
		/* L */
		{ 0x4460, 0x0E80, 0xC440, 0x2E00 },
		COLOR_ORANGE
	},
	{
		/* O */
		{ 0xCC00, 0xCC00, 0xCC00, 0xCC00 },
		COLOR_YELLOW
	},
	{
		/* S */
		{ 0x06C0, 0x8C40, 0x6C00, 0x4620 },
		COLOR_GREEN
	},
	{
		/* T */
		{ 0x0E40, 0x4C40, 0x4E00, 0x4640 },
		COLOR_PURPLE
	},
	{
		/* Z */
		{ 0x0C60, 0x4C80, 0xC600, 0x2640 },
		COLOR_RED
	}
};

/** VARIABLES */

/** Type that stores what piece type a tile in a field belongs to */
typedef u8 FieldType;

/** Current Piece */
static Piece _cp;

/** Next Piece */
static Piece _np;

/** Current game status (see enum above) */
static Status _status;

/** Milliseconds duration for a tile to move down by one tile */
static i32 _ticks_update = FALL_SPEED_DEFAULT;

/** Indicates that the field has changed and should be redrawn */
static bool _changed;

/** Array that stores landed, non-moving pieces */
static FieldType _field[FIELD_SIZE];

/* --- FUNCTIONS --- */

/**
 * @brief Get the piece at a position inside a field
 *
 * @param field The field
 * @param x X-coordinate
 * @param y Y-coordinate
 * @return
 */
static i32 _field_get(FieldType *field, i32 x, i32 y)
{
	return (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT) ? -1 :
		field[y * FIELD_WIDTH + x];
}

/**
 * @brief Clear a row and shift down rows above
 *
 * @param field Playing field
 * @param row The row to remove
 */
static void _field_shift(FieldType *field, i32 row)
{
	i32 x, y;
	for(y = row; y > 0; --y)
	{
		for(x = 0; x < FIELD_WIDTH; ++x)
		{
			field[y * FIELD_WIDTH + x] =
				field[(y - 1) * FIELD_WIDTH + x];
		}
	}
}

/**
 * @brief Remove full rows and update score
 *
 * @param field Playing field
 * @return Score change
 */
static i32 _field_rows(FieldType *field)
{
	i32 x, y, score;

	score = 0;
	for(y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(x = 0; x < FIELD_WIDTH; ++x)
		{
			if(!field[y * FIELD_WIDTH + x])
			{
				break;
			}
		}

		if(x == FIELD_WIDTH)
		{
			_field_shift(field, y);
			score += 100;
		}
	}

	return score;
}

/**
 * @brief Draw field grid
 */
static void _draw_grid(void)
{
	i32 i;
	for(i = 0; i <= BLOCK_SIZE * FIELD_WIDTH; i += BLOCK_SIZE)
	{
		gfx_rect(i, 0, 1, BLOCK_SIZE * FIELD_HEIGHT, COLOR_WHITE);
	}

	for(i = 0; i <= BLOCK_SIZE * FIELD_HEIGHT; i += BLOCK_SIZE)
	{
		gfx_rect(0, i, BLOCK_SIZE * FIELD_WIDTH, 1, COLOR_WHITE);
	}
}

/**
 * @brief Get the next random tetris piece
 *
 * @param p Output parameter for new piece
 * @return Piece type of the following piece
 */
static PieceType _new_piece(Piece *p)
{
	static PieceType next, bag[7];
	static i32 idx = 6;

	if(idx == 6)
	{
		/* create a new shuffled bag of pieces */
		int i, j, tmp;
		for(i = 0; i < 7; ++i)
		{
			bag[i] = i;
		}

		for(i = 0; i < 7; ++i)
		{
			j = rand() % 7;
			tmp = bag[i];
			bag[i] = bag[j];
			bag[j] = tmp;
		}

		idx = 0;
	}

	p->X = 4;
	p->Y = 0;
	p->Rotation = 0;
	p->Type = next;
	next = bag[idx++];
	return next;
}

/**
 * @brief Check if the position of a piece is valid, meaning that it is
 *        not outside of the frame and not colliding with pieces on the field
 *
 * @param field Pointer to field aray
 * @param p Pointer to piece
 * @return true if position is valid, false otherwise
 */
static bool _valid_position(FieldType *field, Piece *p)
{
	i32 bit, row, col;
	u16 blocks;

	row = 0;
	col = 0;
	blocks = Pieces[p->Type].Blocks[p->Rotation];

	for(bit = 0x8000; bit > 0; bit >>= 1)
	{
		if((blocks & bit) && _field_get(field, p->X + col, p->Y + row))
		{
			return false;
		}

		if(++col == 4)
		{
			col = 0;
			++row;
		}
	}

	return true;
}

/**
 * @brief Place a piece into a field
 *
 * @param field Pointer to the field on which to put the piece
 * @param p Pointer to the piece that should be placed
 */
static void _to_field(FieldType *field, Piece *p)
{
	i32 x, y, bit, row, col;
	u16 blocks;

	row = col = 0;
	blocks = Pieces[p->Type].Blocks[p->Rotation];
	for(bit = 0x8000; bit > 0; bit >>= 1)
	{
		if(blocks & bit)
		{
			x = p->X + col;
			y = p->Y + row;

			field[y * FIELD_WIDTH + x] = p->Type + 1;
		}

		if(++col == 4)
		{
			col = 0;
			++row;
		}
	}
}

/**
 * @brief Draw the tetris field to the screen
 */
static void _draw_field(void)
{
	static FieldType new[FIELD_SIZE], prev[FIELD_SIZE];
	i32 x, y, offset;
	PieceType type;

	memcpy(new, _field, sizeof(_field));
	_to_field(new, &_cp);
	//_to_field(new, &_np);
	for(y = 0; y < FIELD_HEIGHT; ++y)
	{
		for(x = 0; x < FIELD_WIDTH; ++x)
		{
			offset = y * FIELD_WIDTH + x;
			type = new[offset];
			Color color = type ? Pieces[type - 1].Fill : COLOR_BLACK;
			if(type != prev[offset])
			{
				gfx_rect(
					x * BLOCK_SIZE + 1,
					y * BLOCK_SIZE + 1,
					BLOCK_SIZE - 1,
					BLOCK_SIZE - 1,
					color);
			}
		}
	}

	memcpy(prev, new, sizeof(_field));
}

/**
 * @brief Main key event handler
 *
 * @param key The key that was pressed
 * @param chr The character corresponding to the key
 * @param state Key state (pressed, repeat, released)
 */
void event_key(Key key, i32 chr, KeyState state)
{
	if(state == KEYSTATE_RELEASED)
	{
		switch(key)
		{
		case KEY_DOWN:
			_ticks_update = FALL_SPEED_DEFAULT;
			break;
		}
	}
	else
	{
		switch(key)
		{
		case KEY_DOWN:
			_ticks_update = FALL_SPEED_FAST;
			break;

		case KEY_UP:
			if(--_cp.Rotation == -1)
			{
				_cp.Rotation = 3;
			}

			if(_valid_position(_field, &_cp))
			{
				if(++_cp.Rotation == 4)
				{
					_cp.Rotation = 0;
				}
			}

			_changed = true;
			break;

		case KEY_LEFT:
			--_cp.X;
			_changed = true;
			if(_valid_position(_field, &_cp))
			{
				++_cp.X;
			}
			break;

		case KEY_RIGHT:
			++_cp.X;
			_changed = true;
			if(_valid_position(_field, &_cp))
			{
				--_cp.X;
			}
			break;
		}
	}

	(void)chr;
}

/**
 * @brief Main setup function, initialize game
 */
void setup(void)
{
	keyboard_register_event(event_key);
	_np.X = 11;
	_np.Y = 0;
	_np.Rotation = 1;
	memset(_field, 0, sizeof(_field));
	_np.Type = _new_piece(&_cp);
	_status = STATUS_RUNNING;
	_draw_grid();
	font_string(210, 10, "Next piece:", font_default, COLOR_WHITE, COLOR_BLACK);
}

/**
 * @brief Main game loop, update state
 */
void loop(void)
{
	static u32 last_ticks, score;
	u32 ticks;

	if((ticks = millis()) > last_ticks + _ticks_update)
	{
		last_ticks = ticks;
		++_cp.Y;
		if(_valid_position(_field, &_cp))
		{
			--_cp.Y;
			_to_field(_field, &_cp);
			score += _field_rows(_field);
			_np.Type = _new_piece(&_cp);
			_ticks_update = FALL_SPEED_DEFAULT;
			if(_valid_position(_field, &_cp))
			{
				memset(_field, 0, sizeof(_field));
			}
		}

		_changed = true;
	}

	if(_changed)
	{
		_changed = false;
		_draw_field();
	}
}
