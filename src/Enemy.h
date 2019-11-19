#pragma once
#include "Types.h"
#include "MoveableObject.h"

class Enemy
	: public MoveableObject
{
public :
	Enemy(float x, float y, color col, float speed, int timeToNextMove);
	int getTimeToNextMove();
	int getTimeToRespawn();
	void decrementActualTimeToNextMove();
	void decrementActualTimeToRespawn();
	bool canMove();
	bool canRespawn();
	void die();
	void draw();
	void respawn();

private :
	point2d respawnPos_;
	int timeToNextMove_;
	int actualTimeToNextMove_;
	int timeToRespawn_;
	int actualTimeToRespawn_;
};
