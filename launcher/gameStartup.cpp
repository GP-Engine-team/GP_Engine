#include "gameStartup.hpp"

#include "Engine/Core/Game/AbstractGame.hpp"
#include "game.hpp"

#include <cassert>

GameStartup::GameStartup() : m_game(createGameInstance())
{

}

void GameStartup::update()
{
	assert(m_game != nullptr);
	std::function<void(double, double)> update = std::bind(&AbstractGame::update, m_game, std::placeholders::_1, std::placeholders::_2);
	std::function<void(double, double)> fixedUpdate = std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
	std::function<void()> render = std::bind(&AbstractGame::render, m_game);
	timeSystem.update(update, fixedUpdate, render);
}

GameStartup::~GameStartup()
{
	destroyGameInstance(m_game);
}