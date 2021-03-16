/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

struct GLFWwindow;

namespace GPE
{ class Scene; }

namespace Editor
{

	class Editor
	{
	private:
		GPE::SceneViewer m_sceneView;
		GLFWwindow* m_window;
		int				 m_framebufferWidth;
		int				 m_framebufferHeight;

<<<<<<< HEAD
	private:
		void setupGLFWWindow();
		void setupDearImGui();
=======
	GPE::Scene& loadDefaultScene() const;

private:
	void setupGLFWWindow  ();
	void setupDearImGui   ();
>>>>>>> 59855cc58431c14d0faaa2c8957fe1439f9c7ffd

		void renderMenuBar() const;
		void renderLevelEditor() const;
		void renderInspector() const;
		void renderSceneGraph(const class GPE::Scene& scene) const;
		void renderExplorer() const;

	public:
		Editor(GLFWwindow* window);

		void update();
		void render();
		bool isRunning();

		friend void windowFramebufferResized(GLFWwindow* window, int width, int height);
	};

} // End of namespace Editor
