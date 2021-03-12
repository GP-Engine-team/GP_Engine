#include "Editor/EditorStartup.hpp"
#include "Editor/Editor.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
<<<<<<< HEAD
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Core/System/SystemsManager.hpp"

#include "imgui/imgui.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

//#include "Game/Game.hpp"
=======
#include "Editor/ExternalDeclarations.hpp"
>>>>>>> f9c92a2f04f232cee164cdc9567ea99f25afb330

namespace Editor
{

void EditorStartup::initDearImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}


EditorStartup::EditorStartup()
    : m_fixedUpdate{[&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
      {        
          if (m_game != nullptr)
              m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
      }},
      m_update{[&](double fixedUnscaledDeltaTime, double deltaTime)
      {
          if (m_game != nullptr)
              m_game->update(fixedUnscaledDeltaTime, deltaTime);
      }},
      m_render{[&]()
      {
          if (m_game != nullptr)
              m_game->render();

          m_editor.update();
          m_editor.render();
      }},
      m_reloadableCpp{"./../projects/GPGame/bin/Release/GPGame.dll"},
      m_editor{GPE::SystemsManager::getInstance()->window.getGLFWWindow()},
      m_game{nullptr}
{
    initDearImGui(GPE::SystemsManager::getInstance()->window.getGLFWWindow());
    ADD_PROCESS(reloadableCpp, createGameInstance);
    ADD_PROCESS(reloadableCpp, destroyGameInstance);
}


EditorStartup::~EditorStartup()
{
    if (m_game != nullptr)
    {
        GET_PROCESS(reloadableCpp, destroyGameInstance)(m_game);
        //destroyGameInstance(m_game);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void EditorStartup::startGame()
{
    if (m_game != nullptr)
    {
        //delete m_game;
        GET_PROCESS(reloadableCpp, destroyGameInstance)(m_game);
    }
    //m_game = createGameInstance();
    auto a = GET_PROCESS(reloadableCpp, createGameInstance);
    m_game = a();
}


void EditorStartup::closeGame()
{
    if (m_game != nullptr)
    {
        //destroyGameInstance(m_game);
        GET_PROCESS(reloadableCpp, destroyGameInstance)(m_game);
        m_game = nullptr;
    }
}


void EditorStartup::update() 
{
    if (m_game != nullptr)
    {
        m_game->update(0, 0);
    }

    timeSystem.update(m_update, m_fixedUpdate, m_render);
    isRunning = m_editor->isRunning();

    if (reloadableCpp.refresh())
    {
        startGame();
    }
}

} // End of namespace Editor