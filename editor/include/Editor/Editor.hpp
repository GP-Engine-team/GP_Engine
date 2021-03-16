/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Intermediate/Viewers/SceneViewer.hpp"

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
		GPE::SceneViewer m_sceneView;
		GLFWwindow* m_window;
		int				 m_framebufferWidth;
		int				 m_framebufferHeight;

		GPE::Scene& loadDefaultScene() const;

		/**
		 * @brief Function that crate scene graph recursively for each node in imGui window.
		 * @param gameObject
		 * @param idElem
		 * @return the pointer to selected game object. Else return null ptr
		*/
		GPE::GameObject* recursifSceneGraphNode(GPE::GameObject& gameObject, int idElem = 0) const;

	private:
		void setupGLFWWindow();
		void setupDearImGui();

		void renderMenuBar() const;
		void renderLevelEditor() const;
		void renderSceneGraph(GPE::Scene& scene) const;
		void renderInspector(GPE::GameObject& gameObject) const;
		void renderExplorer() const;

	public:
		Editor(GLFWwindow* window);

		void update();
		void render();
		bool isRunning();

		friend void windowFramebufferResized(GLFWwindow* window, int width, int height);
	};

} // End of namespace Editor
