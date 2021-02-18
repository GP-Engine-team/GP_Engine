#include "Game.hpp"

#include "Engine/Core/Debug/Assert.hpp"

using namespace GPE;

AbstractGame* createGameInstance()
{
	return new Game();
}

void destroyGameInstance(AbstractGame* game)
{
	GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
	delete game;
}
