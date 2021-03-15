/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

struct GLFWwindow;

namespace Editor
{

class Editor
{
private:
	//GPE::SceneViewer m_sceneView;
	GLFWwindow*		 m_window;
	int				 m_framebufferWidth;
	int				 m_framebufferHeight;

private:
	void setupGLFWWindow  ();
	void setupDearImGui   ();

	void renderMenuBar    () const;
	void renderLevelEditor() const;
	void renderInspector  () const;
	void renderSceneGraph () const;
	void renderExplorer   () const;

public:
	Editor(GLFWwindow* window);

	void setDefaultScene();
	void update			();
	void render			();
	bool isRunning		();

	friend void windowFramebufferResized(GLFWwindow* window, int width, int height);
};

} // End of namespace Editor