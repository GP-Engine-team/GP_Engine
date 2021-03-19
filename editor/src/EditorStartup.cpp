#include "Editor/EditorStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/Core/Rendering/Window/WindowGLFW.hpp"
#include "Engine/Engine.hpp"

#include "imgui/imgui.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

//#include "Game/Game.hpp"
#include "Editor/ExternalDeclarations.hpp"

namespace Editor
{

	GLFWwindow* EditorStartup::initDearImGui(GLFWwindow* window)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");

		return window;
	}


	EditorStartup::EditorStartup()
		: m_fixedUpdate{ [&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
		  {
			  if (m_game != nullptr)
				  m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
		  } },
		m_update{ [&](double fixedUnscaledDeltaTime, double deltaTime)
		{
		  GPE::Engine::getInstance()->inputManager.processInput();
		  if (m_game != nullptr)
			  m_game->update(fixedUnscaledDeltaTime, deltaTime);
		} },
			  m_render{ [&]()
			  {
				  GPE::Engine::getInstance()->renderer.swapBuffer();

				  /*if (m_game != nullptr)
					  m_game->render();*/

				  m_editor.update();
				  m_editor.render();
			  } },
			m_reloadableCpp{ gameDllPath },
				  m_editor{ initDearImGui(GPE::Engine::getInstance()->window.getGLFWWindow()) },
				  m_game{ nullptr }
			  {
				  ADD_PROCESS(m_reloadableCpp, createGameInstance);
				  ADD_PROCESS(m_reloadableCpp, destroyGameInstance);
				  ADD_PROCESS(m_reloadableCpp, setGameEngineInstance);

				  m_reloadableCpp.onUnload = [&]()
				  {
					  closeGame();
				  };


				  GPE::Engine::getInstance()->inputManager.setupCallbacks(GPE::Engine::getInstance()->window.getGLFWWindow());
			  }


			  EditorStartup::~EditorStartup()
			  {
				  if (m_game != nullptr)
				  {
					  auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
					  destroyer(m_game);
				  }

				  ImGui_ImplOpenGL3_Shutdown();
				  ImGui_ImplGlfw_Shutdown();
				  ImGui::DestroyContext();
			  }


			  void EditorStartup::startGame()
			  {
				  if (m_game != nullptr)
				  {
					  auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
					  destroyer(m_game);
				  }
				  auto a = GET_PROCESS(m_reloadableCpp, createGameInstance);
				  m_game = a();
			  }

			  void EditorStartup::closeGame()
			  {
				  if (m_game != nullptr)
				  {
					  auto destroyer = GET_PROCESS(m_reloadableCpp, destroyGameInstance);
					  destroyer(m_game);
					  m_game = nullptr;
				  }
			  }

			  void EditorStartup::update()
			  {
				  if (m_game != nullptr)
				  {
					  m_game->update(0, 0);
				  }

				  GPE::Engine::getInstance()->timeSystem.update(m_update, m_fixedUpdate, m_render);
				  isRunning = m_editor.isRunning();

				  if (m_reloadableCpp.refresh())
				  {
					  auto sync = GET_PROCESS(m_reloadableCpp, setGameEngineInstance);
					  (*sync)(*GPE::Engine::getInstance());
					  startGame();
				  }
			  }

} // End of namespace Editor
