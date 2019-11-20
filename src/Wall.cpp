#include "Wall.h"

Wall::Wall(float x, float y)
	: CollidableObject(x, y, TILE_WIDTH, TILE_HEIGHT, ColorGray)
{
}

void Wall::draw()
{
	draw_rectangle(color_, bounding_);
}