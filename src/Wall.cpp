#include "Wall.h"

Wall::Wall(float x, float y)
	: CollidableObject(x, y, TILE_DIM, TILE_DIM, ColorGray)
{
}

void Wall::draw()
{
	fill_rectangle(color_, bounding_);
	draw_rectangle(ColorBlack, bounding_);
}