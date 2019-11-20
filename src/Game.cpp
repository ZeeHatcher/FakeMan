#include "Game.h"

Game::Game()
{
    initMap();

    player_ = new Player();
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

    player_->draw();
}

void Game::initMap()
{
    // Init "indestructible" borders
    borders_.push_back(new Wall(0, 0, 10 * TILE_DIM, TILE_DIM));
    borders_.push_back(new Wall(0, 1 * TILE_DIM, TILE_DIM, 9 * TILE_DIM));
    borders_.push_back(new Wall(9 * TILE_DIM, 1 * TILE_DIM, TILE_DIM, 9 * TILE_DIM));
    borders_.push_back(new Wall(0, 9 * TILE_DIM, 9 * TILE_DIM, TILE_DIM));

    // Init "destructible" walls
}
