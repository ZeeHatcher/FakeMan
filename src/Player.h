#pragma once
#include "Bomb.h"
#include "Dimensions.h"
#include "MoveableObject.h"
#include "SwinGame.h"

class Player
	: public MoveableObject
{
public :
	Player();
	int getAmmo();
	int getScore();
	void incrementAmmo();
	void incrementScore();
	Bomb* useAmmo();
	void die();
	void draw();

private :
	bitmap bmp_;
	int score_;
	int ammo_;
};
