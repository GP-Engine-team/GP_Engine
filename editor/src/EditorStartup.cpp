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
    GPE::InputManager& inputs = GPE::Engine::getInstance()->inputManager;

    // Default editor-specific inputs
    inputs.bindInput(GLFW_KEY_SPACE,        "up");
    inputs.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    inputs.bindInput(GLFW_KEY_D,            "right");
    inputs.bindInput(GLFW_KEY_A,            "left");
    inputs.bindInput(GLFW_KEY_W,            "forward");
    inputs.bindInput(GLFW_KEY_S,            "backward");
    inputs.bindInput(GLFW_KEY_ESCAPE,       "exit");
    inputs.bindInput(GLFW_KEY_LEFT_SHIFT,   "sprint");

    inputs.setupCallbacks(GPE::Engine::getInstance()->window.getGLFWWindow());
    // GPE::Engine::getInstance()->inputManager.setCursorMode(GPE::Engine::getInstance()->window.getGLFWWindow(),
    //                                                        GLFW_CURSOR_NORMAL);
}

EditorStartup::EditorStartup()
    : m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {}},
      m_update{[&](double unscaledDeltaTime, double deltaTime)
      {
          GPE::Engine::getInstance()->inputManager.processInput();
          Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
          m_editor.update(*this);
      }},
      m_render{[&]()
      {
          m_editor.render();
          GPE::Engine::getInstance()->renderer.swapBuffer();
      }},
      m_editor{initDearImGuiProxy(GPE::Engine::getInstance()->window.getGLFWWindow()),
               GPE::Engine::getInstance()->sceneManager.loadScene("Default scene")},
      m_reloadableCpp{gameDllPath}, m_game{nullptr}
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
    GPE::Engine::getInstance()->timeSystem.clearScaledTimer();
    GPE::Engine::getInstance()->timeSystem.clearUnscaledTimer();

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
        pauseGame();
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
    }

    auto a = GET_PROCESS(m_reloadableCpp, createGameInstance);
    m_game = a();

    m_editor.setSceneInEdition(*GPE::Engine::getInstance()->sceneManager.getCurrentScene());

    if (gameWasInstanciated)
    {
        GPE::Engine::getInstance()->sceneManager.removeScene("Default scene");
    }
}


void EditorStartup::closeGame()
{
    stopGame();

    if (m_game != nullptr)
    {
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
        m_game = nullptr;
    }
    
    // TODO: are the scene previously loaded removed by m_game's destructor?
    m_editor.setSceneInEdition(GPE::Engine::getInstance()->sceneManager.loadScene("Default scene"));
}


void EditorStartup::playGame()
{
    // TODO: update the way editor captures inputs?
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
    {
        m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
    };
    m_update = [&](double unscaledDeltaTime, double deltaTime)
    {
        GPE::Engine::getInstance()->inputManager.processInput();
        m_game->update(unscaledDeltaTime, deltaTime);
        m_editor.update(*this);
    };
}


void EditorStartup::pauseGame()
{
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {};
    m_update = [&](double unscaledDeltaTime, double deltaTime)
    {
        GPE::Engine::getInstance()->inputManager.processInput();
        Engine::getInstance()->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
        m_editor.update(*this);
    };
}


void EditorStartup::stopGame()
{
    pauseGame();
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
        (*sync)(*GPE::Engine::getInstance());

        openGame();
    }

    GPE::Engine::getInstance()->timeSystem.update(m_fixedUpdate, m_update, m_render);
    isRunning = m_editor.isRunning();
}
} // End of namespace Editor
