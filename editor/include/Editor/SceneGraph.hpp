#pragma once

#include "Engine/Intermediate/GameObject.hpp"

namespace Editor
{
	class SceneGraph
	{
	protected:

		void recursiveSceneGraphNode(GPE::GameObject& gameObject, GPE::GameObject*& selectedGameObject, int idElem = 0);

	public:

		void renderAndGetSelected(GPE::GameObject& gameObject, GPE::GameObject*& selectedGameObject);
	};

}
