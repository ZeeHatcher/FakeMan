#pragma once
#include <iostream>
#include "DrawableObject.h"
#include "Status.h"

class Menu
	: public DrawableObject
{
public :
	Menu();
	void draw();
	void updateText(Status status);

private :
	std::string text_;
};
