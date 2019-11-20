#pragma once
#include "Collectible.h"

class Food
	: public Collectible
{
public :
	Food(float x, float y);
	void collect(Player* player);
	void draw();
};
