#include <std.h>
#include <ubuntu_bold.h>
#include <game.h>

Window window = {
	.Title = "Test",
	.OnKey = NULL};

Colider coliderGoal = {
	.x = 0,
	.y = 0,
	.w = 20,
	.h = 20};

Colider coliderKill = {
	.x = 20,
	.y = 0,
	.w = 1,
	.h = 100};

Colider coliderPlayer = {
	.x = 0,
	.y = 0,
	.w = 10,
	.h = 10,
};

void clear()
{
	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
}

void renderPlayer(i32 x, i32 y, Color color)
{
	gfx_rect(x, y, 10, 10, color);
}

void renderCollider(Colider col, Color color)
{
	gfx_rect(col.x, col.y, col.w, col.h, color);
}

bool inColider(Colider col, i32 x, i32 y)
{
	i32 x2 = col.x + col.w;
	i32 y2 = col.y + col.h;
	return (col.x <= x) && (x <= x2) && (col.y <= y) && (y <= y2);
}

bool overlap(Colider c1, Colider c2)
{
	// The rectangles don't overlap if
	// one rectangle's minimum in some dimension
	// is greater than the other's maximum in
	// that dimension.
	i32 c1x2 = c1.x + c1.w;
	i32 c1y2 = c1.y + c1.h;
	i32 c2x2 = c2.x + c2.w;
	i32 c2y2 = c2.y + c2.h;

	bool noOverlap = c1.x > c2x2 ||
					 c2.x > c1x2 ||
					 c1.y > c2y2 ||
					 c2.y > c1y2;

	return !noOverlap;
}

#define H_CENTER (GFX_WIDTH / 2)
#define X_MOVE   (H_CENTER - 5)

#define V_CENTER (GFX_HEIGHT / 2)
#define Y_MOVE   (V_CENTER - 5)

#define DELAY    100

i32 player_x = X_MOVE;
i32 player_y = Y_MOVE;
i32 begin;

bool won = false;
bool alive = true;


void setup(void)
{
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

	coliderPlayer.x = player_x;
	coliderPlayer.y = player_y;

	if (overlap(coliderKill, coliderPlayer))
	{
		font_string(50, 100, "YOU DEAD", ubuntu_bold, COLOR_RED, COLOR_BLACK);
	}
	else
	{
		if (keyboard_is_key_pressed(KEY_W))
		{
			player_y -= 10;
		}
		if (keyboard_is_key_pressed(KEY_S))
		{
			player_y += 10;
		}
		if (keyboard_is_key_pressed(KEY_A))
		{
			player_x -= 10;
		}
		if (keyboard_is_key_pressed(KEY_D))
		{
			player_x += 10;
		}
		clear();
		renderCollider(coliderGoal, COLOR_GREEN);
		renderCollider(coliderKill, COLOR_RED);

		if (overlap(coliderGoal, coliderPlayer) || won)
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