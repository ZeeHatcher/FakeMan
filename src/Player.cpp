#include "Player.h"

Player::Player()
	: MoveableObject(10, 10, 10, 10, ColorYellow, 10)
{
	score_ = 0;
	ammo_ = 0;
}

int Player::getAmmo()
{
	return ammo_;
}

void Player::obtainAmmo()
{
}

Bomb* Player::useAmmo()
{
	return new Bomb(bounding_.x, bounding_.y);
}

void Player::die()
{
	alive_ = false;
}

void Player::draw()
{
}