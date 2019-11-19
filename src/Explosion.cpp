#include "Explosion.h"

Explosion::Explosion(float x, float y)
	: CollidableObject(x, y, 10, 10, ColorRed)
{
	actualTimeToDissipate_ = 10;
}

void Explosion::draw()
{

}