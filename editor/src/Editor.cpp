#include "Editor/Editor.hpp"

#include <string>

#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/SceneManager.hpp"
#include "Engine/Serialization/IInspectable.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include <imgui/imgui.h>

#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/InspectContext.hpp"

using namespace GPE;

// Hint to use GPU if available
extern "C"
{
    _declspec(dllexport) int NvOptimusEnablement                  = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Editor
{

/* ========================== Private methods ========================== */
GPE::Scene& Editor::loadDefaultScene() const
{
    return GPE::Engine::getInstance()->sceneManager.loadScene("Empty scene");
}

void Editor::setupDearImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

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
            // Style editor
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

void Editor::renderGameControlBar()
{
    m_gameControlBar.render(*this);
}

void Editor::renderLevelEditor()
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    ImGui::Begin(m_sceneEditor.pScene->getWorld().getName().c_str(), nullptr, ImGuiWindowFlags_NoBackground);

    if (ImGui::IsMouseClicked(0))
    {
        if (ImGui::IsWindowHovered())
        {
            if (unsigned int idSelectedGameObect = m_sceneEditor.getIDOfSelectedGameObject())
            {
                if (GameObject* const selectionGameObject = Engine::getInstance()
                                                                ->sceneManager.getCurrentScene()
                                                                ->getWorld()
                                                                .getGameObjectCorrespondingToID(idSelectedGameObect))
                {
                    m_inspectedObject = selectionGameObject;
                }
                else
                {
                    GPE::Log::getInstance()->logError(
                        stringFormat("No gameObject corresponding to the id %i", idSelectedGameObect));
                }
            }
            // m_sceneEditor.captureInputs();
        }

        // else
        //{
        //    m_sceneEditor.releaseInputs();
        //}
    }

    const ImVec2 size{ImGui::GetContentRegionAvail()};

    m_sceneEditor.resize(static_cast<int>(size.x), static_cast<int>(size.y));
    m_sceneEditor.render();

    ImGui::Image((void*)(intptr_t)m_sceneEditor.textureID, size, {0.f, 1.f}, {1.f, 0.f});
    ImGui::End();

    ImGui::PopStyleVar(2);
}

void Editor::renderInspector()
{
    ImGui::Begin("Inspector");
    if (m_inspectedObject != nullptr)
    {
        GPE::InspectContext context;
        GPE::DataInspector::inspect(context, *m_inspectedObject);
    }
    else
    {
        ImGui::Text("Select an object to edit its attributes");
    }
    ImGui::End();
}

void Editor::renderSceneGraph()
{
    ImGui::Begin("Scene Graph");

    m_sceneGraph.renderAndGetSelected(Engine::getInstance()->sceneManager.getCurrentScene()->getWorld(),
                                      m_inspectedObject);

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
            m_projectContent.renderAndGetSelected(m_inspectedObject);
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
Editor::Editor(GLFWwindow* window, GPE::Scene& editedScene)
    : m_sceneEditor{editedScene}, m_logInspector{}, m_projectContent{}, m_sceneGraph{},
      m_gameControlBar{}, m_window{window}, m_inspectedObject{nullptr}, m_showAppStyleEditor{false}
{
    glfwMaximizeWindow(window);
    setupDearImGui();

    Log::getInstance()->logCallBack = [&](const char* msg) {
        // Log in console
        std::cout << msg;

        // Log in log inspector
        if (ImGui::GetCurrentContext() != nullptr)
            m_logInspector.addLog(msg);
    };
}

void Editor::setSceneInEdition(GPE::Scene& scene)
{
    m_sceneEditor.bindScene(scene);
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

    renderGameControlBar();
    renderLevelEditor();
    renderSceneGraph();
    renderExplorer();
    renderInspector();
}

void Editor::render()
{
    ImGui::ShowDemoWindow(nullptr);

    ImGui::Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
