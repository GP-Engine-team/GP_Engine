/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "SceneEditor.hpp"
#include "GameViewer.hpp"
#include "GameControlBar.hpp"
#include "LogInspector.hpp"
#include "ProjectContent.hpp"
#include "SceneEditor.hpp"
#include "SceneGraph.hpp"

struct GLFWwindow;

namespace GPE
{
class Scene;
class IInspectable;
class GameObject;
class ReloadableCpp;
} // namespace GPE

namespace Editor
{

class Editor
{
private:
    SceneEditor        m_sceneEditor;
    GameViewer         m_gameViewer;
    LogInspector       m_logInspector;
    ProjectContent     m_projectContent;
    SceneGraph         m_sceneGraph;
    GameControlBar     m_gameControlBar;
    GLFWwindow*        m_window               = nullptr;
    GPE::IInspectable* m_inspectedObject      = nullptr;
    bool               m_showAppStyleEditor   = false;
    bool               m_showImGuiDemoWindows = false;

public:
    GPE::ReloadableCpp* m_reloadableCpp = nullptr;

private:
    GPE::Scene& loadDefaultScene() const;

    void setupDearImGui();

    void renderLog();
    void renderStyleEditor();
    void renderMenuBar();
    void renderGameControlBar();
    void renderLevelEditor();
    void renderGameView(GPE::AbstractGame* game);
    void renderInspector();
    void renderSceneGraph();
    void renderExplorer();

    /**
     * @brief Function that crate scene graph recursively for each node in imGui window.
     * @param gameObject
     * @param idElem
     * @return the pointer to selected game object. Else return null ptr
     */
    void recursiveSceneGraphNode(GPE::GameObject& gameObject, int idElem = 0);

public:
    Editor(GLFWwindow* window, GPE::Scene& editedScene);

    void setSceneInEdition(GPE::Scene& scene);
    void update(GPE::AbstractGame* game);
    void render(GPE::AbstractGame* game);
    bool isRunning();
};

} // End of namespace Editor
