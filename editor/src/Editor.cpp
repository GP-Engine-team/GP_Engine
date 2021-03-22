#include "Editor/Editor.hpp"

#include "Engine/Engine.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Resources/SceneManager.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Intermediate/GameObject.hpp"


#include <imgui/imgui.h>
#include "imgui/imgui_internal.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>
#include <string>

using namespace GPE;

// Hint to use GPU if available
extern "C"
{
	_declspec(dllexport) int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


namespace Editor
{
/* ========================== Private methods ========================== */
GPE::Scene& Editor::loadDefaultScene() const
{
	GPE::SceneManager& sm = GPE::Engine::getInstance()->sceneManager;
	sm.addEmpty("Default scene");
	sm.loadScene("Default scene");

	return *sm.getCurrentScene();
}


void Editor::setupDearImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

void Editor::renderStyleEditor()
{
	ImGui::Begin("Style Editor", &m_showAppStyleEditor);
	ImGui::ShowStyleEditor();
	ImGui::End();
}

void Editor::renderMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		// File
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New");
			ImGui::MenuItem("Open");
			ImGui::MenuItem("Save");
			ImGui::EndMenu();
		}

		// Edit
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Edit something");
			ImGui::EndMenu();
		}

		// View
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Add window"))
			{
				ImGui::MenuItem("Viewport");
				ImGui::MenuItem("Scene graph");
				ImGui::MenuItem("Project browser");
				ImGui::MenuItem("Inspector");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		// Options
		if (ImGui::BeginMenu("Options"))
		{
			//Style editor
			ImGui::MenuItem("Style Editor", NULL, &m_showAppStyleEditor);

			// Menu content
			ImGui::MenuItem("Preferences");
			ImGui::EndMenu();
		}

		// Help
		if (ImGui::BeginMenu("Help"))
		{
			// Menu content
			ImGui::MenuItem("Useful links");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}


void Editor::renderLevelEditor()
{
	// Use the whole window content
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

	ImGui::Begin("Level editor", nullptr, ImGuiWindowFlags_NoBackground);
		const ImVec2 size{ImGui::GetContentRegionAvail()};
		
		m_sceneEditor.resize(static_cast<int>(size.x), static_cast<int>(size.y));

		m_sceneEditor.bindScene(*Engine::getInstance()->sceneManager.getCurrentScene());
		m_sceneEditor.render();

		ImGui::Image((void*)(intptr_t)m_sceneEditor.textureID, size, {.0f, 1.f}, {1.f, .0f});
	ImGui::End();

	ImGui::PopStyleVar(2);
}


void Editor::renderInspector()
{
	ImGui::Begin("Inspector");
	if (m_inspectedObject != nullptr)
	{
		ImGui::Text("Object %s selected", m_inspectedObject->getName().c_str());
	}

	else
	{
		ImGui::Text("Select an object to edit its attributes");
	}
	ImGui::End();
}


void Editor::recursiveSceneGraphNode(GameObject& gameObject, int idElem)
{
	ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	static int nodeClicked = -1;
	static int selectionMask = (1 << 2);

	GameObject* selectedGameObject = nullptr;

	const bool isSelected = (selectionMask & (1 << idElem)) != 0;

	if (isSelected)
		nodeFlag |= ImGuiTreeNodeFlags_Selected;

	if (gameObject.children.empty())
	{
		nodeFlag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());
		if (ImGui::IsItemClicked())
		{
			nodeClicked = idElem;
			m_inspectedObject = &gameObject;
		}
	}
	else
	{
		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());
		if (ImGui::IsItemClicked())
		{
			nodeClicked = idElem;
			m_inspectedObject = &gameObject;
		}

		if (nodeOpen)
		{
			for (auto&& child : gameObject.children)
			{
				recursiveSceneGraphNode(*child.get(), ++idElem);
			}
			ImGui::TreePop();
		}
	}

	if (nodeClicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		if (ImGui::GetIO().KeyCtrl)
			selectionMask ^= (1 << nodeClicked);          // CTRL+click to toggle
		else //if (!(selectionMask & (1 << nodeClicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
			selectionMask = (1 << nodeClicked);           // Click to single-select
	}
}

void Editor::renderSceneGraph()
{
	ImGui::Begin("Scene Graph");
	recursiveSceneGraphNode(Engine::getInstance()->sceneManager.getCurrentScene()->world);

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		ImGui::OpenPopupOnItemClick("SceneGraphContext");

	if (ImGui::BeginPopup("SceneGraphContext"))
	{
		if (ImGui::MenuItem("Remove one", NULL, false)) {}
		if (ImGui::MenuItem("Remove all", NULL, false)) {}
		ImGui::EndPopup();
	}

	ImGui::End();
}

void Editor::renderLog()
{
	ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	m_logInspector.draw("Example: Log");

	ImGui::PopStyleVar();
	ImGui::EndChild();
}

void Editor::renderExplorer()
{
	ImGui::Begin("Explorer");
	if (ImGui::BeginTabBar("Explorer"))
	{
		if (ImGui::BeginTabItem("Project"))
		{
			m_projectContent.render();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Logs"))
		{
			renderLog();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Profiler"))
		{
			ImGui::Text("Plots, and graphs, and numbers...");
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
}




/* ========================== Constructor & destructor ========================== */
Editor::Editor(GLFWwindow* window)
	: m_sceneEditor{loadDefaultScene()},
	  m_window{window}
{
	glfwMaximizeWindow(window);
	setupDearImGui();

	Log::getInstance()->logCallBack = [&](const char* msg)
	{
		// Log in console
		std::cout << msg;

		// Log in log inspector
		m_logInspector.addLog(msg);
	};

}


void Editor::update()
{
	// Initialize a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Start drawing
	if (m_showAppStyleEditor)
	{
		renderStyleEditor();
	}

	renderMenuBar();

	ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

	renderLevelEditor();
	renderSceneGraph();
	renderExplorer();
	renderInspector();
}

void Editor::render()
{
	ImGui::ShowDemoWindow(nullptr);

	ImGui::Render();

	glViewport(0, 0, m_sceneEditor.width, m_sceneEditor.height);
	glClearColor(1.f, 1.f, 1.f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Editor::isRunning()
{
	return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor
