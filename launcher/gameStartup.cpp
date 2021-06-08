#include "GameStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Core/HotReload/SingletonsSync.hpp"
#include "Engine/Engine.hpp"
#include "Game.hpp"
#include <Engine/Intermediate/GameObject.hpp>

#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

using namespace GPE;

static Engine* initEngineProxy()
{
    Engine* const engine = Engine::getInstance();

    setGameEngineInstance(*engine);
    setLogInstance(*Log::getInstance());

    return engine;
}

GameStartup::GameStartup()
    : m_engine{initEngineProxy()}, m_game{createGameInstance()},

      // Make all systems update their components
      m_update{[&](double unscaledDeltaTime, double deltaTime) {
          updateSceneManager();
          m_engine->animSystem.update(deltaTime);
          m_engine->sceneManager.getCurrentScene()->behaviourSystem.update(deltaTime);
          m_engine->sceneManager.getCurrentScene()->sceneRenderer.update(deltaTime);
          m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
          m_engine->inputManager.processInput();
          m_engine->soundSystem.update();

          m_game->update(unscaledDeltaTime, deltaTime);
      }},

      // Update physics
      m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {
          m_engine->sceneManager.getCurrentScene()->behaviourSystem.fixedUpdate(fixedDeltaTime);
          m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);

          m_engine->physXSystem.updateController(fixedDeltaTime);
          m_engine->physXSystem.advance(fixedDeltaTime);
      }},

      // Rendering
      m_render{[&]() {
          int h, w;
          m_engine->window.getSize(w, h);
          m_game->setViewport(.0f, .0f, float(w), float(h));

          glBindFramebuffer(GL_FRAMEBUFFER, 0);
          glViewport(0, 0, w, h);
          m_game->render();

          m_engine->renderer.swapBuffer();
      }}
{
    // ============= Logs =============
    for (auto&& str : Log::getInstance()->getLogs())
    {
        std::cerr << str;
    }

    Log::getInstance()->logCallBack = [&](const char* msg) {
        // Log in console
        std::cerr << msg;
    };

    // ============= Inputs =============
    m_engine->inputManager.setupCallbacks(m_engine->window.getGLFWWindow());
    m_engine->inputManager.setInputMode("Game");

    // ============= Scene =============
    loadFirstScene();

    m_engine->sceneManager.getCurrentScene()->behaviourSystem.onGameAssert = [](const char* msg) {
        GPE_ASSERT(false, msg)
    };
    m_engine->sceneManager.OnSceneChange = std::bind(&GameStartup::startScene, this);
    startScene();

    // ============ Window ==============
    m_engine->window.setFullscreen();
}

void GameStartup::update()
{
    m_engine->timeSystem.update(m_fixedUpdate, m_update, m_render);

    Engine::getInstance()->isRunning = !glfwWindowShouldClose(m_engine->window.getGLFWWindow());
}

GameStartup::~GameStartup()
{
    m_engine->timeSystem.clearScaledTimer();
    m_engine->timeSystem.clearUnscaledTimer();

    destroyGameInstance(m_game);
}

void GameStartup::startScene()
{
    GPE::Engine::getInstance()->sceneManager.getCurrentScene()->behaviourSystem.start();
}
