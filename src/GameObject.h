#pragma once
#include "Types.h"

class GameObject
{
public:
	GameObject(x, y, width, height, col);
	rectangle getBounding();

protected:
	rectangle bounding_;
	color color_;
};
