#include "game.hpp"

#include <cstdio>
#include <cassert>

AbstractGame* createGameInstance()
{
	return new Game();
}

void destroyGameInstance(AbstractGame* game)
{
	assert(game != nullptr);
	delete game;
}
