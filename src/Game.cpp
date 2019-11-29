#include "Game.h"

Game::Game()
{
	player_ = new Player();
	initCells(10, 10);
    initMap();
	initCollectibles();
}

void Game::update()
{
	for (int i = collectibles_.size() - 1; i >= 0; i--)
	{
		Collectible* col = collectibles_[i];

		if (player_->isCollide(col))
		{
			col->collect(player_);
			delete col;
			collectibles_.erase(collectibles_.begin() + i);

			cout << "Score: " << player_->getScore() << endl;
			cout << "Ammo: " << player_->getAmmo() << endl;
			cout << endl;
		}
	}

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

	for (int i = 0; i < collectibles_.size(); i++)
	{
		collectibles_[i]->draw();
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

void Game::initCollectibles()
{
	for (int x = 1; x < cells_.size() - 1; x++)
	{
		for (int y = 1; y < cells_[x].size() - 1; y++)
		{
			bool isOccupied = false;
			rectangle cellBounding = cells_[x][y]->getBounding();

			for (int n = 0; n < walls_.size(); n++)
			{
				rectangle wallBounding = walls_[n]->getBounding();

				if (cellBounding.x == wallBounding.x && cellBounding.y == wallBounding.y)
				{
					isOccupied = true;
					break;
				}
			}

			if (!isOccupied)
			{
				if (x == 1 && y == 1 ||
					x == 8 && y == 1 ||
					x == 1 && y == 8 ||
					x == 8 && y == 8)
				{
					collectibles_.push_back(
						new Ammo(cellBounding.x + cellBounding.width / 2 - 10 / 2, cellBounding.y + cellBounding.height / 2 - 10 / 2)
					);
				}
				else
				{
					collectibles_.push_back(
						new Food(cellBounding.x + cellBounding.width / 2 - 5 / 2, cellBounding.y + cellBounding.height / 2 - 5 / 2)
					);
				}
			}
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
				borders_.push_back(new Wall(cellBounding.x, cellBounding.y, ColorBlue));
			}
		}
		else
		{
			rectangle cellBounding;

			cellBounding = (cells_[x].front())->getBounding();
			borders_.push_back(new Wall(cellBounding.x, cellBounding.y, ColorBlue));

			cellBounding = (cells_[x].back())->getBounding();
			borders_.push_back(new Wall(cellBounding.x, cellBounding.y, ColorBlue));
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