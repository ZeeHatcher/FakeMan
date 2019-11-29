#include "Game.h"

Game::Game()
{
	player_ = new Player();
	initCells(10, 10);
    initMap();
}

void Game::update()
{
	rectangle& playerBounding = player_->getBounding();

	if (key_down(VK_LEFT))
	{
		player_->move(-1, 0);

		for (int i = 0; i < borders_.size(); i++)
		{
			if (player_->isCollide(borders_[i]))
			{
				rectangle wallBounding = borders_[i]->getBounding();
				player_->warp(wallBounding.x + wallBounding.width + 1, playerBounding.y);
			}
		}

		for (int i = 0; i < walls_.size(); i++)
		{
			if (player_->isCollide(walls_[i]))
			{
				rectangle wallBounding = walls_[i]->getBounding();
				player_->warp(wallBounding.x + wallBounding.width + 1, playerBounding.y);
			}
		}
	}

	if (key_down(VK_RIGHT))
	{
		player_->move(1, 0);

		for (int i = 0; i < borders_.size(); i++)
		{
			if (player_->isCollide(borders_[i]))
			{
				rectangle wallBounding = borders_[i]->getBounding();
				player_->warp(wallBounding.x - playerBounding.width - 1, playerBounding.y);
			}
		}

		for (int i = 0; i < walls_.size(); i++)
		{
			if (player_->isCollide(walls_[i]))
			{
				rectangle wallBounding = walls_[i]->getBounding();
				player_->warp(wallBounding.x - playerBounding.width - 1, playerBounding.y);
			}
		}
	}

	if (key_down(VK_UP))
	{
		player_->move(0, -1);

		for (int i = 0; i < borders_.size(); i++)
		{
			if (player_->isCollide(borders_[i]))
			{
				rectangle wallBounding = borders_[i]->getBounding();
				player_->warp(playerBounding.x, wallBounding.y + wallBounding.height + 1);
			}
		}

		for (int i = 0; i < walls_.size(); i++)
		{
			if (player_->isCollide(walls_[i]))
			{
				rectangle wallBounding = walls_[i]->getBounding();
				player_->warp(playerBounding.x, wallBounding.y + wallBounding.height + 1);
			}
		}
	}

	if (key_down(VK_DOWN))
	{
		player_->move(0, 1);

		for (int i = 0; i < borders_.size(); i++)
		{
			if (player_->isCollide(borders_[i]))
			{
				rectangle wallBounding = borders_[i]->getBounding();
				player_->warp(playerBounding.x, wallBounding.y - playerBounding.height - 1);
			}
		}

		for (int i = 0; i < walls_.size(); i++)
		{
			if (player_->isCollide(walls_[i]))
			{
				rectangle wallBounding = walls_[i]->getBounding();
				player_->warp(playerBounding.x, wallBounding.y - playerBounding.height - 1);
			}
		}
	}
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

	player_->draw();
}

void Game::initCells(int widthSpan, int heightSpan)
{
	for (int x = 0; x < widthSpan; x++)
	{
		cells_.push_back(std::vector<Cell*>());
		for (int y = 0; y < heightSpan; y++)
		{
			cells_[x].push_back(new Cell(x * TILE_DIM, y * TILE_DIM));
		}
	}
}

void Game::initMap()
{
    // Init "indestructible" borders
	for (int x = 0; x < cells_.size(); x++)
	{
		if (x == 0 || x == cells_.size() - 1)
		{
			for (int y = 0; y < cells_[x].size(); y++)
			{
				rectangle cellBounding = cells_[x][y]->getBounding();
				borders_.push_back(new Wall(cellBounding.x, cellBounding.y));
			}
		}
		else
		{
			rectangle cellBounding;

			cellBounding = (cells_[x].front())->getBounding();
			borders_.push_back(new Wall(cellBounding.x, cellBounding.y));

			cellBounding = (cells_[x].back())->getBounding();
			borders_.push_back(new Wall(cellBounding.x, cellBounding.y));
		}
	}

    // Init "destructible" walls
	initWalls(2, 2, 5, 0);
	initWalls(2, 7, 5, 0);
	initWalls(2, 4, 0, 1);
	initWalls(7, 4, 0, 1);
	initWalls(4, 5, 1, 0);
}

void Game::initWalls(int x, int y, int widthSpan, int heightSpan)
{
	for (int n = x; n < x + widthSpan + 1; n++)
	{
		if (n >= cells_.size()) break;

		for (int m = y; m < y + heightSpan + 1; m++)
		{
			if (m >= cells_[n].size()) break;

			rectangle cellBounding = cells_[n][m]->getBounding();
			walls_.push_back(new Wall(cellBounding.x, cellBounding.y));	
		}
	}	
}