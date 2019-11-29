#include "Food.h"

Food::Food(float x, float y)
	: Collectible(x, y, 5, 5, ColorYellow)
{
}

void Food::collect(Player* player)
{
	player->incrementScore();
}

void Food::draw()
{
	draw_rectangle(color_, bounding_);
}