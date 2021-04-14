﻿#include "GameStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Engine.hpp"
#include "Game.hpp"
#include "SingletonsSync.hpp"

#include "GLFW/glfw3.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

using namespace GPE;

GameStartup::GameStartup()
{
    setGameEngineInstance(*GPE::Engine::getInstance());
    setContextCurrent(GPE::Engine::getInstance()->window.getGLFWWindow());
    setLogInstance(*GPE::Log::getInstance());

    // setImguiCurrentContext(ImGui::GetCurrentContext());

    for (auto&& str : GPE::Log::getInstance()->getLogs())
    {
        std::cout << str;
    }

    Log::getInstance()->logCallBack = [&](const char* msg) {
        // Log in console
        std::cout << msg;
    };

    m_game = createGameInstance();

    GPE_ASSERT(m_game != nullptr, "m_game should be valid since we're running the game.");
    gameFunctionsPtr.update = [&](double a, double b) {
        GPE::Engine::getInstance()->inputManager.processInput();

        m_game->update(a, b);
    };
    gameFunctionsPtr.fixedUpdate =
        std::bind(&AbstractGame::fixedUpdate, m_game, std::placeholders::_1, std::placeholders::_2);
    gameFunctionsPtr.render = [&]() {
        int h, w;
        GPE::Engine::getInstance()->window.getSize(w, h);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, w, h);
        m_game->render();

        GPE::Engine::getInstance()->renderer.swapBuffer();
    };

    GPE::Engine::getInstance()->inputManager.setupCallbacks(GPE::Engine::getInstance()->window.getGLFWWindow(), true);
    GPE::Engine::getInstance()->inputManager.setCursorMode(GPE::Engine::getInstance()->window.getGLFWWindow(),
                                                           GLFW_CURSOR_DISABLED);
}

void GameStartup::update()
{
    GPE::Engine::getInstance()->timeSystem.update(gameFunctionsPtr.fixedUpdate, gameFunctionsPtr.update,
                                                  gameFunctionsPtr.render);
}

GameStartup::~GameStartup()
{
    GPE::Engine::getInstance()->timeSystem.clearScaledTimer();
    GPE::Engine::getInstance()->timeSystem.clearUnscaledTimer();

    destroyGameInstance(m_game);
}
