#include "Editor/Editor.hpp"

#include "Engine/ECS/System/SystemsManager.hpp"
#include "Engine/Resources/SceneManager.hpp"

#include "imgui/imgui.h"
//#include "imgui/imgui_internal.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include <cstdio>
#include <cstdlib>

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


void Editor::renderLevelEditor() const
{
    ImGui::Begin("Level editor");
        ImGui::Text("Level editor");
        //ImVec2 v{ImGui::GetCurrentWindowRead()->ContentSize};
        // sceneEditor.resize(static_cast<int>(v.x), static_cast<int>(v.y));
        // sceneEditor.render();

    ImGui::End();
}


void Editor::renderInspector() const
{
    ImGui::Begin("Inspector");
        ImGui::Text("Inspector content");
    ImGui::End();
}


void Editor::renderSceneGraph() const
{
    ImGui::Begin("Scene graph");
        ImGui::Text("Scene graph hierarchy");
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
    : /*m_sceneView{*GPE::SystemsManager::getInstance()->sceneManager.getCurrentScene()}, */
      m_window{window}, m_framebufferWidth{0}, m_framebufferHeight{0}
{
    setupGLFWWindow();
    setupDearImGui();
}


void Editor::setDefaultScene()
{

}


void Editor::update()
{
    // Listen to keyboard and mouse
    glfwPollEvents();

    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Start drawing
    renderMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

    renderLevelEditor();
    renderSceneGraph();
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
    
    glfwSwapBuffers(m_window);
}


bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor