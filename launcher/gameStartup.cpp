#include "GameStartup.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Game.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/ECS/System/SystemsManager.hpp"
#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

using namespace GPE;

GameStartup::GameStartup()
{
	setGameSystemsManagerInstance(*GPE::SystemsManager::getInstance());
	m_game = createGameInstance();

	GPE_ASSERT(m_game != nullptr, "m_game should be valid since we're running the game.");
	gameFunctionsPtr.update = [&](double a, double b)
	{
		GPE::SystemsManager::getInstance()->inputManager.processInput();
		m_game->update(a, b);
	};
	gameFunctionsPtr.fixedUpdate = std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
	gameFunctionsPtr.render = [&]()
	{
		GPE::SystemsManager::getInstance()->renderer.swapBuffer();
		m_game->render();
	};

	GPE::SystemsManager::getInstance()->inputManager.setupCallbacks(GPE::SystemsManager::getInstance()->window.getGLFWWindow(), true);
	GPE::SystemsManager::getInstance()->inputManager.setCursorMode(GPE::SystemsManager::getInstance()->window.getGLFWWindow(), GLFW_CURSOR_DISABLED);
}

void GameStartup::update()
{
	GPE::SystemsManager::getInstance()->timeSystem.update(gameFunctionsPtr.update, gameFunctionsPtr.fixedUpdate, gameFunctionsPtr.render);
}

GameStartup::~GameStartup()
{
	destroyGameInstance(m_game);
}
