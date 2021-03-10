#include "Game.hpp"

#include "Engine/Core/Debug/Assert.hpp"

using namespace GPE;

extern "C" AbstractGame* createGameInstance()
{
	return new Game();
}

extern "C" void destroyGameInstance(AbstractGame* game)
{
	GPE_ASSERT(game != nullptr, "m_editor should be valid since we've just ran the editor.");
	delete game;
}

void Game::update(double unscaledDeltaTime, double deltaTime) 
{

}