#include <std.h>
#include <ubuntu_bold.h>
#include <game.h>

Window window = {
	.Title = "Test",
	.OnKey = NULL};

Colider colider = {
	.x = 0,
	.y = 0,
	.w = 20,
	.h = 20};

void clear()
{
	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
}

void renderPlayer(i32 x, i32 y, Color color)
{
	gfx_rect(x, y, 10, 10, color);
}

void sleep(i32 mills)
{
	i32 begin = millis();
	while (millis() - begin < mills)
	{
	}
	return;
}

void renderCollider(Colider col)
{
	gfx_rect(col.x, col.y, col.w, col.h, COLOR_GREEN);
}

bool colliding(Colider col, i32 x, i32 y)
{
	i32 x2 = col.x + col.w;
	i32 y2 = col.y + col.h;
	return (col.x <= x) && (x <= x2) && (col.y <= y) && (y <= y2);
}

i32 main(void)
{
	window_open(&window);

	i32 h_center = GFX_WIDTH / 2;
	i32 x_move = h_center - 5;

	i32 v_center = GFX_HEIGHT / 2;
	i32 y_move = v_center - 5;

	i32 player_x = x_move;
	i32 player_y = y_move;

	bool won = false;

	clear();

	loop
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
		renderCollider(colider);
		if (colliding(colider, player_x, player_y) || won)
		{
			renderPlayer(player_x, player_y, COLOR_BLUE);
			won = true;
			font_string(50, 100, "YOU WON", ubuntu_bold, COLOR_RED, COLOR_BLACK);
		}
		else
		{
			renderPlayer(player_x, player_y, COLOR_RED);
		}
		sleep(100);
	}

	return 0;
}
