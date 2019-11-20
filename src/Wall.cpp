#include "Wall.h"

Wall::Wall(float x, float y)
	: Wall(x, y, TILE_DIM, TILE_DIM)
{
}

Wall::Wall(float x, float y, int32_t width, int32_t height)
	: CollidableObject(x, y, width, height, ColorGray)
{
}

void Wall::draw()
{
	fill_rectangle(color_, bounding_);
	draw_rectangle(ColorBlack, bounding_);
}