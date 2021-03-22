#include "Editor/SceneGraph.hpp"
#include <imgui/imgui.h>

using namespace Editor;
using namespace GPE;


static void controlPreviousItem(GPE::GameObject& gameObject, GameObject*& selectedGameObject, int idElem)
{
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		selectedGameObject = &gameObject;
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("SceneGraphContext" + idElem);
	}

	if (ImGui::BeginPopup("SceneGraphContext" + idElem))
	{
		ImGui::Text(gameObject.getName().c_str());
		if (ImGui::MenuItem("Add child", NULL, false))
		{
			gameObject.addChild<GameObject>(GameObject::CreateArg{ "NewGameObject" });
		}

		if (ImGui::MenuItem("Remove", NULL, false))
		{
			gameObject.destroy();
		}
		ImGui::EndPopup();
	}
}

void SceneGraph::recursiveSceneGraphNode(GPE::GameObject& gameObject, GameObject*& selectedGameObject, int idElem)
{
	ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_Selected * (selectedGameObject == &gameObject) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (gameObject.children.empty())
	{
		nodeFlag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());

		controlPreviousItem(gameObject, selectedGameObject, idElem);
	}
	else
	{
		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());

		controlPreviousItem(gameObject, selectedGameObject, idElem);

		if (nodeOpen)
		{
			for (auto&& child : gameObject.children)
			{
				recursiveSceneGraphNode(*child.get(), selectedGameObject, ++idElem);
			}
			ImGui::TreePop();
		}
	}
}

void SceneGraph::renderAndGetSelected(GPE::GameObject& gameObject, GameObject*& selectedGameObject)
{
	recursiveSceneGraphNode(gameObject, selectedGameObject);
}