#include "MoveableObject.h"

MoveableObject::MoveableObject(float x, float y, int32_t width, int32_t height, color col, float speed)
	: CollidableObject(x, y, width, height, col)
{
	speed_ = speed;
	alive_ = true;
}

void MoveableObject::move(float x, float y)
{
}

bool MoveableObject::getAlive()
{
	return alive_;
}