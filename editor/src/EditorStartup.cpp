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

#include "Engine/Core/HotReload/SingletonsSync.hpp"

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
    m_engine->inputManager.bindInput(GLFW_KEY_SPACE, "up");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_CONTROL, "down");
    m_engine->inputManager.bindInput(GLFW_KEY_D, "right");
    m_engine->inputManager.bindInput(GLFW_KEY_A, "left");
    m_engine->inputManager.bindInput(GLFW_KEY_W, "forward");
    m_engine->inputManager.bindInput(GLFW_KEY_S, "backward");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_SHIFT, "sprint");
    m_engine->inputManager.bindInput(GLFW_KEY_LEFT_SHIFT, "walk");

    m_engine->inputManager.setupCallbacks(m_engine->window.getGLFWWindow());
}

EditorStartup::EditorStartup()
    : m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {}},
      m_update{[&](double unscaledDeltaTime, double deltaTime) {
          m_engine->sceneManager.update();
          m_engine->inputManager.processInput();
          m_editor.update(*this);
          m_engine->sceneManager.getCurrentScene()->sceneRenderer.updateDebug(deltaTime);
          m_engine->sceneManager.getCurrentScene()->behaviourSystem.updateEditor(deltaTime);
          m_engine->sceneManager.getCurrentScene()->sceneRenderer.update(deltaTime);
          m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
      }},
      m_render{[&]() {
          m_editor.render();
          m_engine->renderer.swapBuffer();
      }},
      m_editor{initDearImGuiProxy(GPE::Engine::getInstance()->window.getGLFWWindow()),
               GPE::Engine::getInstance()->sceneManager.setCurrentScene("Default scene")},
      m_reloadableCpp{gameDllPath}, m_game{nullptr}, m_engine{GPE::Engine::getInstance()}
{
    m_editor.reloadableCpp = &m_reloadableCpp;

    ADD_PROCESS(m_reloadableCpp, createGameInstance);
    ADD_PROCESS(m_reloadableCpp, destroyGameInstance);
    ADD_PROCESS(m_reloadableCpp, setGameEngineInstance);
    ADD_PROCESS(m_reloadableCpp, setLogInstance);
    ADD_PROCESS(m_reloadableCpp, setImguiCurrentContext);
    ADD_PROCESS(m_reloadableCpp, getGameUIContext);
    ADD_PROCESS(m_reloadableCpp, getGameUIContext);
    ADD_PROCESS(m_reloadableCpp, saveSceneToPath);
    ADD_PROCESS(m_reloadableCpp, savePrefabToPath);
    ADD_PROCESS(m_reloadableCpp, loadSceneFromPath);
    ADD_PROCESS(m_reloadableCpp, getAllComponentsName);
    ADD_PROCESS(m_reloadableCpp, getComponentClass);
    ADD_PROCESS(m_reloadableCpp, getAllComponentsClasses);
    ADD_PROCESS(m_reloadableCpp, createGameObject);
    ADD_PROCESS(m_reloadableCpp, destroyGameObject);
    ADD_PROCESS(m_reloadableCpp, createComponentByName);
    ADD_PROCESS(m_reloadableCpp, createComponentByID);
    ADD_PROCESS(m_reloadableCpp, destroyComponent);
    ADD_PROCESS(m_reloadableCpp, loadPrefabFromPath);
    ADD_PROCESS(m_reloadableCpp, clonePrefab);
    ADD_PROCESS(m_reloadableCpp, loadFirstScene);
    ADD_PROCESS(m_reloadableCpp, updateSceneManager);
    ADD_PROCESS(m_reloadableCpp, loadPrefabFromString);
    ADD_PROCESS(m_reloadableCpp, savePrefabToString);

    m_reloadableCpp.onUnload = [&]() { closeGame(); };

    initializeDefaultInputs();
}

EditorStartup::~EditorStartup()
{
    m_engine->timeSystem.clearScaledTimer();
    m_engine->timeSystem.clearUnscaledTimer();

    m_editor.unbindCurrentScene();
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
        m_editor.unbindCurrentScene();
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
    }

    auto a = GET_PROCESS(m_reloadableCpp, createGameInstance);
    m_game = a();

    Engine::getInstance()->resourceManager.add<Shader>("gameObjectIdentifier",
                                                       "./resources/shaders/vGameObjectIdentifier.vs",
                                                       "./resources/shaders/fGameObjectIdentifier.fs");

    auto loadFirstSceneFunct = GET_PROCESS(m_reloadableCpp, loadFirstScene);
    loadFirstSceneFunct();
    m_editor.setSceneInEdition(loadFirstSceneFunct());
    m_editor.saveFolder = Engine::getInstance()->sceneManager.firstLoadedScene.parent_path().string();

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
    m_engine->exit = [&]() { m_editor.releaseGameInputs(); };
}

void EditorStartup::closeGame()
{
    //// TODO: are the scene previously loaded removed by m_game's destructor?
    // m_editor.setSceneInEdition(m_engine->sceneManager.loadScene("Default scene"));
    m_editor.unbindCurrentScene();

    if (m_game != nullptr)
    {
        stopGame();
        m_editor.unbindCurrentScene();
        auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
        destroyer(m_game);
        m_game = nullptr;
    }

    // TODO: are the scenes previously loaded removed by m_game's destructor?
    m_editor.setSceneInEdition(m_engine->sceneManager.setCurrentScene("Default scene"));

    // There is no more active game, replace m_engine->exit to something not dependant on m_game
    m_engine->exit = [&]() { m_engine->window.close(); };
}

void EditorStartup::playGame()
{
    m_engine->sceneManager.getCurrentScene()->behaviourSystem.onGameAssert = [&](const char* msg) {
        Log::getInstance()->logError(msg);
        stopGame();
    };
    Engine::getInstance()->sceneManager.OnSceneChange = std::bind(&EditorStartup::startScene, this);

    m_editor.gameViewer.lockInputToGame();

    if (m_game->state == EGameState::STOPPED)
    {
        m_editor.saveCurrentScene();
    }

    // Do not change the order of instructions inside the lambdas
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {
        if (m_game->state == EGameState::PLAYING)
        {
            m_engine->physXSystem.advance(fixedDeltaTime);
            m_engine->sceneManager.getCurrentScene()->behaviourSystem.fixedUpdate(fixedDeltaTime);

            m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
        }
    };

    m_update = [&](double unscaledDeltaTime, double deltaTime) {
        if (m_game->state == EGameState::PLAYING)
        {
            auto updateSceneManagerFunct = GET_PROCESS(m_reloadableCpp, updateSceneManager);
            updateSceneManagerFunct();
            m_engine->inputManager.processInput();

            m_engine->sceneManager.getCurrentScene()->behaviourSystem.update(deltaTime);
            m_engine->sceneManager.getCurrentScene()->sceneRenderer.updateDebug(deltaTime);
            m_engine->sceneManager.getCurrentScene()->behaviourSystem.updateEditor(deltaTime);
            m_engine->sceneManager.getCurrentScene()->sceneRenderer.update(deltaTime);
            m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();

            m_game->update(unscaledDeltaTime, deltaTime);

            // Editor MUST be the last element update because user can stop the game. If the game stops it should not be
            // updated
            m_editor.update(*this);
        }
    };

    m_game->state = EGameState::PLAYING;
    startScene();
}

void EditorStartup::pauseGame()
{
    // Do not change the order of instructions inside the lambdas
    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {};
    m_update      = [&](double unscaledDeltaTime, double deltaTime) {
        if (m_game->state == EGameState::PAUSED)
        {
            auto updateSceneManagerFunct = GET_PROCESS(m_reloadableCpp, updateSceneManager);
            updateSceneManagerFunct();
            m_engine->inputManager.processInput();
            m_engine->sceneManager.getCurrentScene()->sceneRenderer.updateDebug(deltaTime);
            m_engine->sceneManager.getCurrentScene()->behaviourSystem.updateEditor(deltaTime);
            m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();

            m_editor.update(*this);
        }
    };

    m_game->state = EGameState::PAUSED;
}

void EditorStartup::stopGame()
{
    if (m_game->state == EGameState::STOPPED)
        return;

    m_engine->sceneManager.getCurrentScene()->behaviourSystem.startOnBehaviourAdd = false;
    m_engine->sceneManager.getCurrentScene()->behaviourSystem.onGameAssert        = nullptr;
    Engine::getInstance()->sceneManager.OnSceneChange                             = nullptr;

    m_fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime) {};
    m_update      = [&](double unscaledDeltaTime, double deltaTime) {
        if (m_game->state == EGameState::STOPPED)
        {
            auto updateSceneManagerFunct = GET_PROCESS(m_reloadableCpp, updateSceneManager);
            updateSceneManagerFunct();

            m_engine->inputManager.processInput();
            m_editor.update(*this);
            m_engine->sceneManager.getCurrentScene()->sceneRenderer.updateDebug(deltaTime);
            m_engine->sceneManager.getCurrentScene()->behaviourSystem.updateEditor(deltaTime);
            m_engine->sceneManager.getCurrentScene()->sceneRenderer.update(deltaTime);

            m_engine->sceneManager.getCurrentScene()->getWorld().updateSelfAndChildren();
        }
    };

    m_editor.reloadCurrentScene();
    m_game->state = EGameState::STOPPED;
}

void EditorStartup::update()
{
    if (m_reloadableCpp.refresh())
    {
        auto syncLog = GET_PROCESS(m_reloadableCpp, setLogInstance);
        (*syncLog)(*GPE::Log::getInstance());

        auto sync = GET_PROCESS(m_reloadableCpp, setGameEngineInstance);
        (*sync)(*m_engine);

        m_editor.inspectedObject = nullptr;

        openGame();
    }

    m_engine->timeSystem.update(m_fixedUpdate, m_update, m_render);

    // Check if user try to stop game in editor
    if (!Engine::getInstance()->isRunning)
    {
        Engine::getInstance()->isRunning = true;
        stopGame();
    }
    Engine::getInstance()->isRunning = m_editor.isRunning();
}

void EditorStartup::startScene()
{
    m_editor.setSceneInEdition(*GPE::Engine::getInstance()->sceneManager.getCurrentScene());
    GPE::Engine::getInstance()->sceneManager.getCurrentScene()->behaviourSystem.start();
}

} // End of namespace Editor
