/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include "LogInspector.hpp"
#include "ProjectContent.hpp"
#include "SceneGraph.hpp"
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
		GPE::SceneViewer m_sceneEditor;
		GLFWwindow*		 m_window;
		GPE::GameObject* m_inspectedObject;
<<<<<<< HEAD
		int							  m_framebufferWidth;
		int							  m_framebufferHeight;
		bool					      m_showAppStyleEditor = false;
		LogInspector			      m_logInspector;
		ProjectContent				  m_projectContent;
		SceneGraph					  m_sceneGraph;
=======
		bool			 m_showAppStyleEditor = false;
		LogInspector	 m_logInspector;
		ProjectContent	 m_projectContent;
>>>>>>> 44b876b24b1c23175463c313b7bf27dc1d2eaff6

		GPE::Scene& loadDefaultScene() const;

	private:
		void setupDearImGui();

		void renderLog();
		void renderStyleEditor();
		void renderMenuBar();
		void renderLevelEditor();
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
		Editor(GLFWwindow* window);

		void update();
		void render();
		bool isRunning();
	};

} // End of namespace Editor
