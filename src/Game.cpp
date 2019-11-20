#include "Game.h"

Game::Game()
{
    initMap();
}

void Game::update()
{

}

void Game::draw()
{
    for (int i = 0; i < borders_.size(); i++)
    {
        borders_[i]->draw();
    }
	
	for (int i = 0; i < walls_.size(); i++)
	{
		walls_[i]->draw();
	}
}

void Game::initMap()
{
    // Init "indestructible" borders
	initWalls(&borders_, 0, 0, 9, 0);
	initWalls(&borders_, 0, 9, 9, 0);
	initWalls(&borders_, 0, 1, 0, 7);
	initWalls(&borders_, 9, 1, 0, 7);

    // Init "destructible" walls
	initWalls(&walls_, 2, 2, 5, 0);
	initWalls(&walls_, 2, 7, 5, 0);
	initWalls(&walls_, 2, 4, 0, 1);
	initWalls(&walls_, 7, 4, 0, 1);
	initWalls(&walls_, 4, 5, 1, 0);
}

void Game::initWalls(std::vector<Wall*>* walls, int x, int y, int widthSpan, int heightSpan)
{
	for (int n = x; n < x + widthSpan + 1; n++)
	{
		for (int m = y; m < y + heightSpan + 1; m++)
		{
			walls->push_back(new Wall(n*TILE_DIM, m*TILE_DIM));	
		}
	}	
}