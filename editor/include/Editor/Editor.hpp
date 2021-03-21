﻿/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"
#include <vector>

struct GLFWwindow;

namespace GPE
{
	class Scene;
	class GameObject;
}

namespace Editor
{

class Editor
{
private:
	GPE::SceneViewer			  m_sceneEditor;
	GLFWwindow*					  m_window;
	GPE::GameObject*			  m_inspectedObject;
	int							  m_framebufferWidth;
	int							  m_framebufferHeight;

	GPE::Scene& loadDefaultScene() const;

private:
	void setupGLFWWindow  ();
	void setupDearImGui   ();

	void renderMenuBar    () const;
	void renderLevelEditor();
	void renderInspector  ();
	void renderSceneGraph ();
	void renderExplorer   () const;

	/**
		* @brief Function that crate scene graph recursively for each node in imGui window.
		* @param gameObject
		* @param idElem
		* @return the pointer to selected game object. Else return null ptr
	*/
	void recursiveSceneGraphNode(GPE::GameObject& gameObject, int idElem = 0);

public:
	Editor(GLFWwindow* window);

	void update();
	void render();
	bool isRunning();

	friend void windowFramebufferResized(GLFWwindow* window, int width, int height);
};

} // End of namespace Editor
