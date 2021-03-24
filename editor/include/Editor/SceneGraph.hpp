#pragma once

#include "Engine/Intermediate/GameObject.hpp"

namespace Editor
{
	class DeferedSetParent
	{
		GPE::GameObject* m_movedGO = nullptr;
		GPE::GameObject* m_newParentGO = nullptr;

	public:

		void bind(GPE::GameObject& movedGO, GPE::GameObject& newParentGO);

		void tryExecute();
	};

	class SceneGraph
	{
	protected:
		DeferedSetParent deferedSetParent;

	protected:

		void controlPreviousItem(GPE::GameObject& gameObject, GPE::GameObject*& selectedGameObject, int idElem);

		void recursiveSceneGraphNode(GPE::GameObject& gameObject, GPE::GameObject*& selectedGameObject, int idElem = 0);

	public:

		void renderAndGetSelected(GPE::GameObject& gameObject, GPE::GameObject*& selectedGameObject);
	};

}
