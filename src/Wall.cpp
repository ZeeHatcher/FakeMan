#include "Wall.h"

Wall::Wall(float x, float y)
	: CollidableObject(x, y, TILE_DIM, TILE_DIM, ColorGray)
{
}

void Wall::draw()
{
	draw_rectangle(color_, bounding_);
	// draw_rectangle(ColorDarkGray, bounding_);
}