#pragma once
#include "CollidableObject.h"

class Wall
	: public CollidableObject
{
public :
	Wall(float x, float y);
	void draw();
};
