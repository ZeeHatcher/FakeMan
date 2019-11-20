#pragma once
#include "Collectible.h"

class Ammo
	: public Collectible
{
public :
	Ammo(float x, float y);
	void collect(Player* player);
	void draw();
};
