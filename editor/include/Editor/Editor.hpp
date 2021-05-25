/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "GameControlBar.hpp"
#include "GameViewer.hpp"
#include "LogInspector.hpp"
#include "ProjectContent.hpp"
#include "SceneEditor.hpp"
#include "SceneGraph.hpp"

struct GLFWwindow;

namespace GPE
{
class AbstractGame;
class IInspectable;
class GameObject;
class ReloadableCpp;
class SavedScene;
class Scene;
} // namespace GPE

namespace Editor
{

class EditorStartup;

class Editor
{

public:
    SceneEditor         sceneEditor;
    GameViewer          gameViewer;
    LogInspector        logInspector;
    ProjectContent      projectContent;
    SceneGraph          sceneGraph;
    GameControlBar      gameControlBar;
    std::string         saveFolder;
    GPE::IInspectable*  inspectedObject;
    GPE::ReloadableCpp* reloadableCpp;
private:
    GLFWwindow*         m_window;

public:
    bool                showAppStyleEditor;
    bool                showImGuiDemoWindows;

private:
    void setupDearImGui();

    void renderLog();
    void renderStyleEditor();
    void renderMenuBar();
    void renderGameControlBar(EditorStartup& startup);
    void renderLevelEditor();
    void renderGameView(EditorStartup& startup);
    void renderInspector();
    void renderSceneGraph();
    void renderExplorer();

public:
    Editor(GLFWwindow* window, GPE::Scene& editedScene);

    void setSceneInEdition(GPE::Scene& scene);
    void releaseGameInputs();
    void update(EditorStartup& startup);
    void render();
    bool isRunning();

    // Removes Editor elements from the scene before saving
    void saveScene(GPE::Scene* scene, const char* path);
    // Removes Editor elements from the scene before loading
    void loadScene(GPE::Scene* scene, const char* path);

    void saveCurrentScene();
    void reloadCurrentScene();
    void unbindCurrentScene();
};

} // End of namespace Editor
