#include "GameStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Engine.hpp"
#include "Game.hpp"
#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

using namespace GPE;

GameStartup::GameStartup()
{
    setGameEngineInstance(*GPE::Engine::getInstance());
    m_game = createGameInstance();

    GPE_ASSERT(m_game != nullptr, "m_game should be valid since we're running the game.");
    gameFunctionsPtr.update = [&](double a, double b) {
        GPE::Engine::getInstance()->inputManager.processInput();
        m_game->update(a, b);
    };
    gameFunctionsPtr.fixedUpdate =
        std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
    gameFunctionsPtr.render = [&]() {
        GPE::Engine::getInstance()->renderer.swapBuffer();
        m_game->render();
    };

    GPE::Engine::getInstance()->inputManager.setupCallbacks(GPE::Engine::getInstance()->window.getGLFWWindow(), true);
    GPE::Engine::getInstance()->inputManager.setCursorMode(GPE::Engine::getInstance()->window.getGLFWWindow(),
                                                           GLFW_CURSOR_DISABLED);
}

void GameStartup::update()
{
    GPE::Engine::getInstance()->timeSystem.update(gameFunctionsPtr.update, gameFunctionsPtr.fixedUpdate,
                                                  gameFunctionsPtr.render);
}

GameStartup::~GameStartup()
{
    destroyGameInstance(m_game);
}
