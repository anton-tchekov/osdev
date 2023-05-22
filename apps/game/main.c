#include <std.h>
#include <ubuntu_bold.h>

#define H_CENTER      (GFX_WIDTH / 2)
#define X_MOVE        (H_CENTER - 5)

#define V_CENTER      (GFX_HEIGHT / 2)
#define Y_MOVE        (V_CENTER - 5)

#define DELAY      100

static Window window =
{
	.Title = "Game",
	.OnKey = NULL
};

static Rectangle colliderGoal =
{
	.X = 0,
	.Y = 0,
	.W = 20,
	.H = 20
};

static Rectangle colliderKill =
{
	.X = 20,
	.Y = 0,
	.W = 1,
	.H = 100
};

static Rectangle colliderPlayer =
{
	.X = 0,
	.Y = 0,
	.W = 10,
	.H = 10
};

static i32 player_x = X_MOVE;
static i32 player_y = Y_MOVE;
static i32 begin;
static bool won = false;
static bool _keys[4];

/*
Rectangle *colliders[] =
{
	&colliderGoal,
	&(Rectangle){10, 40, 50, 60}
};
*/

void clear(void)
{
	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
}

void renderPlayer(i32 x, i32 y, Color color)
{
	gfx_rect(x, y, 10, 10, color);
}

void renderCollider(Rectangle *col, Color color)
{
	gfx_rect(col->X, col->Y, col->W, col->H, color);
}

bool overlap(Rectangle *c1, Rectangle *c2)
{
	/*
	 * The rectangles don't overlap if
	 * one rectangle's minimum in some dimension
	 * is greater than the other's maximum in
	 * that dimension.
	 */

	i32 c1x2 = c1->X + c1->W;
	i32 c1y2 = c1->Y + c1->H;
	i32 c2x2 = c2->X + c2->W;
	i32 c2y2 = c2->Y + c2->H;

	bool noOverlap = c1->X > c2x2 ||
					c2->X > c1x2 ||
					c1->Y > c2y2 ||
					c2->Y > c1y2;

	return !noOverlap;
}

void event_key(Key key, i32 chr, KeyState state)
{
	bool press = state != KEYSTATE_RELEASED;
	switch(key)
	{
		case KEY_W:
			_keys[0] = press;
			break;

		case KEY_S:
			_keys[1] = press;
			break;

		case KEY_A:
			_keys[2] = press;
			break;

		case KEY_D:
			_keys[3] = press;
			break;
	}

	(void)chr;
}

void setup(void)
{
	keyboard_register_event(event_key);
	window_open(&window);
	clear();
}

void loop(void)
{
	if(millis() - begin < DELAY)
	{
		return;
	}

	begin = millis();

	colliderPlayer.X = player_x;
	colliderPlayer.Y = player_y;

	if(overlap(&colliderKill, &colliderPlayer))
	{
		font_string(50, 100, "YOU DEAD", ubuntu_bold, COLOR_RED, COLOR_BLACK);
	}
	else
	{
		if(_keys[0])
		{
			player_y -= 10;
		}

		if(_keys[1])
		{
			player_y += 10;
		}

		if(_keys[2])
		{
			player_x -= 10;
		}

		if(_keys[3])
		{
			player_x += 10;
		}

		clear();
		renderCollider(&colliderGoal, COLOR_GREEN);
		renderCollider(&colliderKill, COLOR_RED);

		if(overlap(&colliderGoal, &colliderPlayer) || won)
		{
			renderPlayer(player_x, player_y, COLOR_BLUE);
			won = true;
			font_string(50, 100, "YOU WON", ubuntu_bold, COLOR_RED, COLOR_BLACK);
		}
		else
		{
			renderPlayer(player_x, player_y, COLOR_WHITE);
		}
	}
}