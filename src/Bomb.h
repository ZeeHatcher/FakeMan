#pragma once
#include "DrawableObject.h"
#include "SwinGame.h"

class Bomb
	: public DrawableObject
{
public :
	Bomb();
	Bomb(float x, float y);
	int getActualTimeToExplode();
	bool canExplode();
	void decrementActualTimeToExplode();
	void explode();
	void draw();

private :
	int actualTimeToExplode_;
};
