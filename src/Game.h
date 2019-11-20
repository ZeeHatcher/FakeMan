#pragma once
#include <vector>
#include "Ammo.h"
#include "Bomb.h"
#include "Collectible.h"
#include "CollidableObject.h"
#include "DrawableObject.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Food.h"
#include "GameObject.h"
#include "Menu.h"
#include "MoveableObject.h"
#include "Player.h"
#include "Status.h"
#include "Wall.h"

class Game
{
public :
	Game();
	void update();
	void draw();

private :
	void initMap();
	void initWalls(std::vector<Wall*>* walls, int x, int y, int widthSpan, int heightSpan);
	
	Menu menu_;
	Player* player_;
	Status status_;
	std::vector<Bomb*> bombs_;
	std::vector<Collectible*> collectibles_;
	std::vector<Enemy*> enemies_;
	std::vector<Explosion*> explosions_;
	std::vector<Wall*> borders_;
	std::vector<Wall*> walls_;
};
