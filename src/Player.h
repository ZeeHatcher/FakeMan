#pragma once
#include "Bomb.h"
#include "MoveableObject.h"
#include "SwinGame.h"

class Player
	: public MoveableObject
{
public :
	Player();
	int getAmmo();
	void obtainAmmo();
	Bomb* useAmmo();
	void die();
	void draw();

private :
	int score_;
	int ammo_;
};
