#pragma once
#include "CollidableObject.h"
#include "Dimensions.h"
#include "SwinGame.h"

class Wall
	: public CollidableObject
{
public :
	Wall(float x, float y);
	Wall(float x, float y, int32_t width, int32_t height);
	void draw();
};
