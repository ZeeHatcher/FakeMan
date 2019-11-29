#include "Ammo.h"

Ammo::Ammo(float x, float y)
	: Collectible(x, y, 10, 10, ColorOrange)
{
}

void Ammo::collect(Player* player)
{
	player->incrementAmmo();
}

void Ammo::draw()
{
	draw_rectangle(color_, bounding_);
}