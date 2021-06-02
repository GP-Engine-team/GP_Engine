#include <Editor/EditorStartup.hpp>

// Engine
#include <Engine/Core/Game/AbstractGame.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Core/HotReload/SingletonsSync.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Resources/SceneManager.hpp>
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/FileExplorer.hpp>
#include <Engine/Serialization/IInspectable.hpp>
#include <Engine/Serialization/InspectContext.hpp>

// Editor
#include <Editor/StylePanel.hpp>

// Third-party
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
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
void Editor::setupDearImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiLoadStyle(PATH_UI_STYLE, ImGui::GetStyle());
}

void Editor::renderStyleEditor()
{
    if (showAppStyleEditor)
    {
        if (ImGui::Begin("Style Editor", &showAppStyleEditor))
        {
            ShowStyleEditor(*this);
        }
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
            if (ImGui::MenuItem("New"))
            {
                projectContent.createNewScene();
            }

            // if (ImGui::MenuItem("Open"))
            //{
            //    sceneEditor.view.unbindScene();
            //    Scene& scene = Engine::getInstance()->sceneManager.setCurrentScene(fileName);
            //    loadScene(&scene, path.c_str());
            //}

            if (ImGui::MenuItem("Save to"))
            {
                saveFolder                 = openFolderExplorerAndGetRelativePath(L"Save location").string().c_str();
                std::filesystem::path path = saveFolder;
                path /= Engine::getInstance()->sceneManager.getCurrentScene()->getName() + ".GPScene";

                saveScene(sceneEditor.view.pScene, path.string().c_str());
            }

            if (ImGui::MenuItem("Save"))
            {
                saveCurrentScene();
            }

            if (ImGui::MenuItem((std::string("Select first scene : ") +
                                 Engine::getInstance()->sceneManager.firstLoadedScene.string())
                                    .c_str()))
            {
                Engine::getInstance()->sceneManager.firstLoadedScene =
                    openFileExplorerAndGetRelativePath(L"Select Scene", {{L"Scene", L"*.GPScene"}}).string().c_str();
            }

            ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit"))
        {
            /*ImGui::MenuItem("Edit something");*/
            ImGui::EndMenu();
        }

        // View
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Viewport");
                ImGui::MenuItem("Scene graph");
                ImGui::MenuItem("Project browser");
                ImGui::MenuItem("Inspector");

                ImGui::MenuItem("Shadow map", nullptr, &showShadowMap);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Layout"))
            {
                if (ImGui::MenuItem("Default"))
                {
                    ImGui::LoadIniSettingsFromDisk("Layout/defaultGUILayout.ini");
                }
                if (ImGui::MenuItem("Quick save"))
                {
                    ImGui::SaveIniSettingsToDisk("Layout/userGUILayout.ini");
                }
                if (ImGui::MenuItem("Quick laod"))
                {
                    ImGui::LoadIniSettingsFromDisk("Layout/userGUILayout.ini");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        // Options
        if (ImGui::BeginMenu("Options"))
        {
            // Style editor
            ImGui::MenuItem("Style Editor", NULL, &showAppStyleEditor);

            // Menu content
            ImGui::MenuItem("Preferences");
            ImGui::EndMenu();
        }

        // Help
        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Demo ImGui", NULL, &showImGuiDemoWindows);

            if (ImGui::BeginMenu("Shortcut"))
            {
                ImGui::Text("WASD : Move");
                ImGui::Text("Shift : Move fast");
                ImGui::Separator();
                ImGui::Text("Ctrl + C : Copy current game object");
                ImGui::Text("Ctrl + V : Past in selection");
                ImGui::Text("Ctrl + S : Save");
                ImGui::Text("Delete : Delete selected game object");
                ImGui::Separator();
                ImGui::Text("F1 : Set default layout");
                ImGui::Text("F2 : Refreash asset");
                ImGui::Text("F5 : Start");
                ImGui::Text("F6 : Pause");
                ImGui::Text("F7 : Stop");

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize("FPS : 144").x -
                             ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::Text("FPS : %0.0f", ImGui::GetIO().Framerate);

        ImGui::EndMainMenuBar();
    }
}

void Editor::renderGameControlBar(EditorStartup& startup)
{
    gameControlBar.render(startup);
}

void Editor::renderLevelEditor()
{
    sceneEditor.view.update();
    sceneEditor.render(*this);
}

void Editor::renderGameView(EditorStartup& startup)
{
    gameViewer.render(startup);
}

void Editor::renderInspector()
{
    if (inspectedObject)
    {
        if (GameObject* pGo = dynamic_cast<GameObject*>(inspectedObject))
        {
            if (pGo->isDead())
            {
                inspectedObject = nullptr;
            }
        }
    }

    if (ImGui::Begin("Inspector"))
    {
        if (inspectedObject != nullptr)
        {
            GPE::InspectContext context;
            GPE::DataInspector::inspect(context, *inspectedObject);
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

        sceneGraph.renderAndGetSelected(root, inspectedObject);
    }

    ImGui::End();
}

void Editor::renderLog()
{
    if (ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        logInspector.draw("Example: Log");

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
                projectContent.renderAndGetSelected(inspectedObject);
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

void Editor::renderShadowMap()
{
    if (!showShadowMap)
        return;

    ImGui::Begin("Shadow map viewer", &showShadowMap);

    if (Engine::getInstance()->sceneManager.getCurrentScene()->sceneRenderer.getShadowMap())
    {

        const ImVec2 size{ImGui::GetContentRegionAvail()};
        ImGui::Image((void*)(intptr_t)Engine::getInstance()
                         ->sceneManager.getCurrentScene()
                         ->sceneRenderer.getShadowMap()->depthMap,
                     size, {.0f, 1.f}, {1.f, .0f});
    }
    else
    {
        const char*  text     = "No shadow map active";
        const ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2       winSize{ImGui::GetWindowSize()};
        winSize.x = (winSize.x - textSize.x) * .5f;
        winSize.y = (winSize.y - textSize.y) * .5f;

        ImGui::SetCursorPos({winSize.x, winSize.y});
        ImGui::Text(text);
    }

    ImGui::End();
}

void Editor::saveScene(GPE::Scene* scene, const char* path)
{
    auto saveFunc = GET_PROCESS((*reloadableCpp), saveSceneToPath);
    saveFunc(scene, path, GPE::SavedScene::EType::XML);
}

void Editor::loadScene(GPE::Scene* scene, const char* path)
{
    inspectedObject = nullptr;

    auto loadFunc = GET_PROCESS((*reloadableCpp), loadSceneFromPath);
    loadFunc(scene, path);

    sceneEditor.view.bindScene(*scene);
    scene->getWorld().pOwnerScene->sceneRenderer.setDefaultMainCamera();
}

void Editor::saveCurrentScene()
{
    if (saveFolder.empty())
        saveFolder = openFolderExplorerAndGetRelativePath(L"Save location").string().c_str();

    std::filesystem::path path = saveFolder;
    path /= Engine::getInstance()->sceneManager.getCurrentScene()->getName() + ".GPScene";

    saveScene(sceneEditor.view.pScene, path.string().c_str());
}

void Editor::reloadCurrentScene()
{
    GPE::Scene* currentScene = sceneEditor.view.pScene;
    if (currentScene == nullptr)
        return;
    std::filesystem::path path = saveFolder;
    path /= currentScene->getName() + ".GPScene";

    if (std::filesystem::exists(path))
    {
        // TODO: need to be replace when children is std::unique_ptr
        for (auto&& child : currentScene->getWorld().children)
        {
            delete child;
        }
        currentScene->getWorld().children.clear();

        sceneEditor.view.unbindScene();
        inspectedObject = nullptr;

        void (*const loadFunc)(GPE::Scene*, const char*) = GET_PROCESS((*reloadableCpp), loadSceneFromPath);
        loadFunc(currentScene, path.string().c_str());

        sceneEditor.view.bindScene(*currentScene);
    }
    else
    {
        saveCurrentScene();
        Log::getInstance()->logError(
            stringFormat("Path \"%s\" dosn't exist to reload scene. Use current save to prevent it's suppression",
                         path.string().c_str()));
    }
}

void Editor::unbindCurrentScene()
{
    sceneEditor.view.unbindScene();
}

/* ========================== Constructor & destructor ========================== */
Editor::Editor(GLFWwindow* window, GPE::Scene& editedScene)
    : sceneEditor(editedScene), gameViewer{}, logInspector{}, projectContent(*this),
      sceneGraph(*this), gameControlBar{}, saveFolder{}, m_window{window}, inspectedObject{nullptr},
      showAppStyleEditor{false}, showImGuiDemoWindows{false}
{
    glfwMaximizeWindow(window);
    setupDearImGui();
}

void Editor::setSceneInEdition(GPE::Scene& scene)
{
    sceneEditor.view.bindScene(scene);
}

void Editor::releaseGameInputs()
{
    gameViewer.lockInputToEditor();
}

void Editor::updateKeyboardShorthand(EditorStartup& startup)
{
    if (ImGui::IsKeyPressed(GLFW_KEY_F1))
    {
        ImGui::LoadIniSettingsFromDisk("Layout/defaultGUILayout.ini");
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_F2))
    {
        projectContent.refreshResourcesList();
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_F5))
    {
        startup.playGame();
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_F6))
    {
        startup.pauseGame();
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_F7))
    {
        startup.stopGame();
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_DELETE) && !ImGui::IsAnyItemActive())
    {
        if (GameObject* pGo = dynamic_cast<GameObject*>(inspectedObject))
        {
            inspectedObject = nullptr;
            pGo->destroy();
        }
    }

    if (!sceneEditor.view.capturingInputs() && ImGui::IsKeyPressed(GLFW_KEY_S) &&
        (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
    {
        saveCurrentScene();
    }

    if (!sceneEditor.view.capturingInputs() && ImGui::IsKeyPressed(GLFW_KEY_C) &&
        (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
    {
        if (GameObject* inspectedGo = dynamic_cast<GameObject*>(inspectedObject))
        {
            auto savePrefabToPathFunct = GET_PROCESS((*reloadableCpp), savePrefabToString);
            paperPress                 = savePrefabToPathFunct(*inspectedGo, SavedScene::EType::XML);
        }
    }

    if (!sceneEditor.view.capturingInputs() && ImGui::IsKeyPressed(GLFW_KEY_V) &&
        (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_CONTROL)))
    {
        if (paperPress.size())
        {
            GameObject* pParent = dynamic_cast<GameObject*>(inspectedObject);
            if (!pParent)
                pParent = &Engine::getInstance()->sceneManager.getCurrentScene()->getWorld();
            auto loadPrefabFromStringFunct = GET_PROCESS((*reloadableCpp), loadPrefabFromString);
            loadPrefabFromStringFunct(*pParent, paperPress, SavedScene::EType::XML);
        }
    }
}

void Editor::update(EditorStartup& startup)
{
    auto syncImGui  = GET_PROCESS((*reloadableCpp), setImguiCurrentContext);
    auto syncGameUI = GET_PROCESS((*reloadableCpp), getGameUIContext);

    // Initialize a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    if (OnUIBeginFrame)
        OnUIBeginFrame(*this);

    ImGui::NewFrame();

    ImGuiContext* gameContext = (syncGameUI == nullptr) ? nullptr : syncGameUI();
    syncImGui(ImGui::GetCurrentContext());

    // Start drawing
    if (showAppStyleEditor)
    {
        renderStyleEditor();
    }

    // Editor
    updateKeyboardShorthand(startup);

    renderMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport());

    renderGameControlBar(startup);
    renderLevelEditor();
    renderSceneGraph();
    renderExplorer();
    renderInspector();
    renderShadowMap();

    if (showImGuiDemoWindows)
    {
        ImGui::ShowDemoWindow(&showImGuiDemoWindows);
    }

    // Game
    syncImGui(gameContext);
    renderGameView(startup);
}

void Editor::render()
{
    ImGui::Render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
    glViewport(0, 0, sceneEditor.view.width, sceneEditor.view.height);
    glClearColor(1.f, 1.f, 1.f, .0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Editor::isRunning()
{
    return !glfwWindowShouldClose(m_window);
}

} // End of namespace Editor
