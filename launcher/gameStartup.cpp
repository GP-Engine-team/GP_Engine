#include "GameStartup.hpp"

#include "Engine/Core/Game/AbstractGame.hpp"
#include "Game.hpp"

#include "Engine/Core/Debug/Assert.hpp"

GameStartup::GameStartup() : m_game(createGameInstance())
{

}

void GameStartup::update()
{
	GPE_ASSERT(m_game != nullptr, "m_game should be valid since we're running the game.");
	std::function<void(double, double)> update = std::bind(&AbstractGame::update, m_game, std::placeholders::_1, std::placeholders::_2);
	std::function<void(double, double)> fixedUpdate = std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
	std::function<void()> render = std::bind(&AbstractGame::render, m_game);
	timeSystem.update(update, fixedUpdate, render);
}

GameStartup::~GameStartup()
{
	destroyGameInstance(m_game);
}