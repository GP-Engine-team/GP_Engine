#include "gameStartup.hpp"

#include "Engine/Core/Game/AbstractGame.hpp"
#include "game.hpp"

#include <cassert>

GameStartup::GameStartup() : game(createGameInstance())
{

}

void GameStartup::update()
{
	assert(game != nullptr);
	game->update();
}

GameStartup::~GameStartup()
{
	destroyGameInstance(game);
}