﻿#include "Editor/Editor.hpp"

// Engine
#include "Engine/Core/Game/AbstractGame.hpp"
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "Engine/Resources/SceneManager.hpp"

// Editor
#include "Editor/ExternalDeclarations.hpp"
#include "Engine/Core/HotReload/ReloadableCpp.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "Engine/Serialization/IInspectable.hpp"
#include "Engine/Serialization/InspectContext.hpp"

// Third-party
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <imgui/imgui.h>

using namespace GPE;

// Hint to use GPU if available
extern "C"
{
    _declspec(dllexport) int NvOptimusEnablement                  = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Editor
{

using namespace GPE;

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
    // io.ConfigViewportsNoAutoMerge   = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void Editor::renderStyleEditor()
{
    if (m_showAppStyleEditor)
    {
        ImGui::Begin("Style Editor", &m_showAppStyleEditor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
}

void Editor::renderMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        // File
        if (ImGui::BeginMenu("File"))
        {
            std::string fileName = "NewScene";
            std::string path = (std::filesystem::current_path() / RESOURCES_DIR).string() + '/' + fileName + ".GPScene";

            if (ImGui::MenuItem("New"))
            {
                Scene& scene = Engine::getInstance()->sceneManager.addEmpty(fileName.c_str());
                saveScene(&scene, path.c_str());
            }

            if (ImGui::MenuItem("Open"))
            {
                Scene&      scene     = Engine::getInstance()->sceneManager.addEmpty(fileName);
                loadScene(&scene, path.c_str());
                Engine::getInstance()->sceneManager.loadScene(fileName);
            }

            if (ImGui::MenuItem("Save"))
            {
                saveScene(m_sceneEditor.view.pScene, path.c_str());
            }

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
            ImGui::MenuItem("Demo ImGui", NULL, &m_showImGuiDemoWindows);

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
    m_sceneEditor.render(m_inspectedObject);
}

void Editor::renderGameView(GPE::AbstractGame* game)
{
    m_gameViewer.render(game);
}

void Editor::renderInspector()
{
    if (ImGui::Begin("Inspector"))
    {
        if (m_inspectedObject != nullptr)
        {
            GPE::InspectContext context;
            GPE::DataInspector::inspect(context, *m_inspectedObject);
        }
        else
        {
            ImGui::Text("Select an object to edit its attributes");
        }
    }
    ImGui::End();
}

void Editor::renderSceneGraph()
{
    if (ImGui::Begin("Scene Graph"))
    {
        GPE::GameObject& root{Engine::getInstance()->sceneManager.getCurrentScene()->getWorld()};
        m_sceneGraph.renderAndGetSelected(root, m_inspectedObject);
    }

    ImGui::End();
}

void Editor::renderLog()
{
    if (ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        m_logInspector.draw("Example: Log");

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
}

void Editor::renderExplorer()
{
    if (ImGui::Begin("Explorer"))
    {
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
    }
    ImGui::End();
}

void Editor::saveScene(GPE::Scene* scene, const char* path)
{
    GPE::Scene* currentScene = m_sceneEditor.view.pScene;
    m_sceneEditor.view.unbindScene();

    auto saveFunc = GET_PROCESS((*m_reloadableCpp), saveSceneToPath);
    saveFunc(scene, path, GPE::SavedScene::EType::XML);

    m_sceneEditor.view.bindScene(*currentScene);
}
void Editor::loadScene(GPE::Scene* scene, const char* path)
{
    m_sceneEditor.view.unbindScene();
    m_inspectedObject = nullptr;

    auto loadFunc = GET_PROCESS((*m_reloadableCpp), loadSceneFromPath);
    loadFunc(scene, path);

    m_sceneEditor.view.bindScene(*scene);
}

/* ========================== Constructor & destructor ========================== */
Editor::Editor(GLFWwindow* window, GPE::Scene& editedScene)
    : m_sceneEditor{editedScene}, m_gameViewer{}, m_logInspector{}, m_projectContent{}, m_sceneGraph{},
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

    m_projectContent.editor = this;
}

void Editor::setSceneInEdition(GPE::Scene& scene)
{
    m_sceneEditor.view.bindScene(scene);
}

void Editor::update(GPE::AbstractGame* game)
{
    auto syncImGui  = GET_PROCESS((*m_reloadableCpp), setImguiCurrentContext);
    auto syncGameUI = GET_PROCESS((*m_reloadableCpp), getGameUIContext);

    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiContext* gameContext = syncGameUI();
    syncImGui(ImGui::GetCurrentContext());

    // Start drawing
    if (m_showAppStyleEditor)
    {
        renderStyleEditor();
    }

    // Editor
    renderMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

    renderGameControlBar();
    renderLevelEditor();
    renderSceneGraph();
    renderExplorer();
    renderInspector();

    if (m_showImGuiDemoWindows)
        ImGui::ShowDemoWindow(&m_showImGuiDemoWindows);

    // Game
    syncImGui(gameContext);
    renderGameView(game);
}

void Editor::render()
{
    ImGui::Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    glViewport(0, 0, m_sceneEditor.view.width, m_sceneEditor.view.height);
    glClearColor(1.f, 1.f, 1.f, .0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor
