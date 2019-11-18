#include "GameObject.h"

GameObject::GameObject(x, y, width, height, col)
{
	bounding_.x = x;
	bounding_.y = y;
	bounding_.width = width;
	bounding_.height = height;
	color_ = col;
}