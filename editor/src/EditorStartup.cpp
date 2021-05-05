#include <Editor/EditorStartup.hpp>
#include <Engine/Core/Debug/Assert.hpp>
#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/Core/Rendering/Window/WindowGLFW.hpp>
#include <Engine/Engine.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <Editor/ExternalDeclarations.hpp>

namespace Editor
{

using namespace GPE;

GLFWwindow* EditorStartup::initDearImGuiProxy(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return window;
}

void EditorStartup::initializeDefaultInputs() const
{
    // Default editor-specific input bindings
    m_engine->inputManager.bindInput(GLFW_KEY_SPACE,        "up");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    m_engine->inputManager.bindInput(GLFW_KEY_D,            "right");
    m_engine->inputManager.bindInput(GLFW_KEY_A,            "left");
    m_engine->inputManager.bindInput(GLFW_KEY_W,            "forward");
    m_engine->inputManager.bindInput(GLFW_KEY_S,            "backward");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_SHIFT,   "sprint");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_SHIFT,   "walk");

    m_engine->inputManager.setupCallbacks(m_engine->window.getGLFWWindow());
}

EditorStartup::EditorStartup()
    : m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {}},
      m_update{[&](double unscaledDeltaTime, double deltaTime)
      {
          m_engine->inputManager.processInput();
          m_editor.update(*this);

          m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
      }},
      m_render{[&]()
      {
          m_editor.render();
          m_engine->renderer.swapBuffer();
      }},
      m_editor       {initDearImGuiProxy(GPE::Engine::getInstance()->window.getGLFWWindow()),
                      GPE::Engine::getInstance()->sceneManager.loadScene("Default scene")},
      m_reloadableCpp{gameDllPath},
      m_game         {nullptr},
      m_engine       {GPE::Engine::getInstance()}
{
    m_editor.m_reloadableCpp = &m_reloadableCpp;

    ADD_PROCESS(m_reloadableCpp, createGameInstance);
    ADD_PROCESS(m_reloadableCpp, destroyGameInstance);
    ADD_PROCESS(m_reloadableCpp, setGameEngineInstance);
    ADD_PROCESS(m_reloadableCpp, setLogInstance);
    ADD_PROCESS(m_reloadableCpp, setImguiCurrentContext);
    ADD_PROCESS(m_reloadableCpp, getGameUIContext);
    ADD_PROCESS(m_reloadableCpp, saveScene);
    ADD_PROCESS(m_reloadableCpp, loadScene);
    ADD_PROCESS(m_reloadableCpp, getGameUIContext);
    ADD_PROCESS(m_reloadableCpp, saveCurrentScene);
    ADD_PROCESS(m_reloadableCpp, loadCurrentScene);

    m_reloadableCpp.onUnload = [&]() { closeGame(); };

    initializeDefaultInputs();
}

EditorStartup::~EditorStartup()
{
    m_engine->timeSystem.clearScaledTimer();
    m_engine->timeSystem.clearUnscaledTimer();

    if (m_game != nullptr)
    {
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorStartup::openGame()
{
    const bool gameWasInstanciated = m_game != nullptr;
    if (gameWasInstanciated)
    {
        stopGame();
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
    }

    auto a = GET_PROCESS(m_reloadableCpp, createGameInstance);
    m_game = a();

    m_editor.setSceneInEdition(*m_engine->sceneManager.getCurrentScene());

    if (gameWasInstanciated)
    {
        m_engine->sceneManager.removeScene("Default scene");
    }

    { // Reset cursor state
        GPE::InputManager& io = m_engine->inputManager;
        io.setCursorLockState(false);
        io.setCursorTrackingState(false);
    }

    // Ending the game must not close the whole engine when the editor is opened
    // The editor handles exiting the whole application itself
    m_engine->exit = [&]()
    {
        stopGame();
        m_editor.releaseGameInputs();
    };
}

void EditorStartup::closeGame()
{
    if (m_game != nullptr)
    {
        stopGame();

        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
        m_game = nullptr;
    }

    // TODO: are the scene previously loaded removed by m_game's destructor?
    m_editor.setSceneInEdition(m_engine->sceneManager.loadScene("Default scene"));

    // There is no more active game, replace m_engine->exit to something not dependant on m_game
    m_engine->exit = [&](){ m_engine->window.close(); };
}

void EditorStartup::playGame()
{
    // Do not change the order of instructions inside the lambdas
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
    {
        m_engine->physXSystem.advance(fixedDeltaTime);
        m_engine->behaviourSystem.fixedUpdate(fixedDeltaTime);

        m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
    };
    m_update = [&](double unscaledDeltaTime, double deltaTime)
    {
        m_engine->inputManager.processInput();
        m_editor.update(*this);

        m_engine->behaviourSystem.update(deltaTime);
        m_engine->sceneManager.getCurrentScene()->sceneRenderer.update(deltaTime);
        m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();

        m_game->update(unscaledDeltaTime, deltaTime);
    };

    m_game->state = EGameState::PLAYING;
}

void EditorStartup::pauseGame()
{
    // Do not change the order of instructions inside the lambdas
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {};
    m_update      = [&](double unscaledDeltaTime, double deltaTime)
    {
        m_engine->inputManager.processInput();
        m_editor.update(*this);

        m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
    };

    m_game->state = EGameState::PAUSED;
}

void EditorStartup::stopGame()
{
    pauseGame();
    // TODO: reload scene
    m_game->state = EGameState::STOPPED;
}

GPE::AbstractGame& EditorStartup::game() const
{
    return *m_game;
}

void EditorStartup::update()
{
    if (m_reloadableCpp.refresh())
    {
        auto syncLog = GET_PROCESS(m_reloadableCpp, setLogInstance);
        (*syncLog)(*GPE::Log::getInstance());

        auto sync = GET_PROCESS(m_reloadableCpp, setGameEngineInstance);
        (*sync)(*m_engine);

        openGame();
    }

    m_engine->timeSystem.update(m_fixedUpdate, m_update, m_render);
    isRunning = m_editor.isRunning();
}
} // End of namespace Editor
