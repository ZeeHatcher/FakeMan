#include "Player.h"

Player::Player()
	: MoveableObject(TILE_DIM+1, TILE_DIM+1, TILE_DIM-2, TILE_DIM-2, ColorYellow, 2)
{
	score_ = 0;
	ammo_ = 0;
	bmp_ = load_bitmap("pacman.png");
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
	draw_rectangle(color_, bounding_);
}