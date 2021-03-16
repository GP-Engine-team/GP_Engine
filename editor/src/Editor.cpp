#include "Editor/Editor.hpp"

#include "Engine/ECS/System/SystemsManager.hpp"
#include "Engine/Resources/SceneManager.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Intermediate/GameObject.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>

using namespace GPE;

// Hint to use GPU if available
extern "C"
{
	_declspec(dllexport) int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


namespace Editor
{

/* ========================== Friend function ========================== */
void windowFramebufferResized(GLFWwindow* window, int newWidth, int newHeight)
{
    Editor* editor{(Editor*)glfwGetWindowUserPointer(window)};

    editor->m_framebufferWidth  = newWidth;
    editor->m_framebufferHeight = newHeight;
}




/* ========================== Private methods ========================== */
GPE::Scene& Editor::loadDefaultScene() const
{
    GPE::SceneManager& sm = GPE::SystemsManager::getInstance()->sceneManager;
    sm.addEmpty("Default scene");
    sm.loadScene("Default scene");

    return *sm.getCurrentScene();
}


void Editor::setupGLFWWindow()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, windowFramebufferResized);
    glfwGetFramebufferSize(m_window, &m_framebufferWidth, &m_framebufferHeight);

    glfwMaximizeWindow(m_window);
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


void Editor::renderMenuBar() const
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
    ImGui::Begin("Level editor");
        const ImVec2 levelEditorSize{ImGui::GetCurrentWindow()->ContentRegionRect.GetSize()};
        //m_sceneView.resize(static_cast<int>(levelEditorSize.x), static_cast<int>(levelEditorSize.y));
        m_sceneView.render();
        ImGui::Image((void*)(intptr_t)m_sceneView.texture.getID(), levelEditorSize);
    ImGui::End();
}


void Editor::renderInspector() const
{
    ImGui::Begin("Inspector");
        ImGui::Text("Inspector content");
    ImGui::End();
}

static void recursifTreeCreation(const GameObject& gameObject, int idElem = 0)
{
	ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	static int nodeClicked = -1;
	int selectionMask = (1 << 2);

	if (gameObject.children.empty())
	{
		nodeFlag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());
		if (ImGui::IsItemClicked())
			nodeClicked = idElem;
	}
	else
	{
		const bool isSelected = (selectionMask & (1 << idElem)) != 0;

		if (isSelected)
			nodeFlag |= ImGuiTreeNodeFlags_Selected;

		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());
		if (ImGui::IsItemClicked())
			nodeClicked = idElem;

		if (nodeOpen)
		{
			for (auto&& child : gameObject.children)
			{
				recursifTreeCreation(*child.get(), ++idElem);
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

void Editor::renderSceneGraph(const Scene& scene) const
{
	ImGui::Begin("Scene Graph");

	recursifTreeCreation(scene.world);

	ImGui::End();
}

void Editor::renderExplorer() const
{
    ImGui::Begin("Explorer");
        if (ImGui::BeginTabBar("Explorer"))
        {
            if (ImGui::BeginTabItem("Project"))
            {
                ImGui::Text("Project's content");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Logs"))
            {
                ImGui::Text("This is a log");
                ImGui::Text("And another log");
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
    : m_sceneView{loadDefaultScene(), 400, 400},
      m_window{window}, m_framebufferWidth{0}, m_framebufferHeight{0}
{
    setupGLFWWindow();
    setupDearImGui();
}


void Editor::update()
{
    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Start drawing
    renderMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

    renderLevelEditor();
	
	if (SystemsManager::getInstance()->sceneManager.getCurrentScene() != nullptr)
		renderSceneGraph(*SystemsManager::getInstance()->sceneManager.getCurrentScene());

    renderExplorer();
    renderInspector();
}


void Editor::render()
{
    ImGui::Render();

    glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
    glClearColor(1.f, 1.f, 1.f, .0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor
