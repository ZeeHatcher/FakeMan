#include "Bomb.h"

Bomb::Bomb()
	: Bomb(10, 10)
{
}

Bomb::Bomb(float x, float y)
	: DrawableObject(x, y, 10, 10, ColorBlack)
{
	actualTimeToExplode_ = 10;
}

void Bomb::draw()
{

}