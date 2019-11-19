#include "Enemy.h"

Enemy::Enemy(float x, float y, color col, float speed, int timeToNextMove)
	: MoveableObject(x, y, 100, 100, col, speed)
{
	respawnPos_.x = x;
	respawnPos_.y = y;
	timeToNextMove_ = timeToNextMove;
	actualTimeToNextMove_ = timeToNextMove;
	timeToRespawn_ = actualTimeToRespawn_ = 100;
}

void Enemy::draw()
{

}

void Enemy::die()
{
	alive_ = false;
}