#include "GameStartup.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Game.hpp"
#include "Engine/Core/Debug/Assert.hpp"

GameStartup::GameStartup() : m_game(createGameInstance())
{
	GPE_ASSERT(m_game != nullptr, "m_game should be valid since we're running the game.");

	gameFunctionsPtr.update			= std::bind(&AbstractGame::update, m_game, std::placeholders::_1, std::placeholders::_2);
	gameFunctionsPtr.fixedUpdate	= std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
	gameFunctionsPtr.render			= std::bind(&AbstractGame::render, m_game);
}

void GameStartup::update()
{
	timeSystem.update(gameFunctionsPtr.update, gameFunctionsPtr.fixedUpdate, gameFunctionsPtr.render);
}

GameStartup::~GameStartup()
{
	destroyGameInstance(m_game);
}