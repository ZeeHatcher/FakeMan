#pragma once
#include "CollidableObject.h"
#include "SwinGame.h"

class Wall
	: public CollidableObject
{
public :
	Wall(float x, float y);
	void draw();
};
