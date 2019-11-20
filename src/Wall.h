#pragma once
#include "CollidableObject.h"
#include "Dimensions.h"
#include "SwinGame.h"

class Wall
	: public CollidableObject
{
public :
	Wall(float x, float y);
	void draw();
};
