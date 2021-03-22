#include "Editor/SceneGraph.hpp"
#include <imgui/imgui.h>

using namespace Editor;
using namespace GPE;

void SceneGraph::recursiveSceneGraphNode(GPE::GameObject& gameObject, GameObject*& selectedGameObject, int idElem)
{
	ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (gameObject.children.empty())
	{
		nodeFlag |= ImGuiTreeNodeFlags_Selected * (selectedGameObject == &gameObject);

		nodeFlag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
		ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());
		if (ImGui::IsItemClicked())
		{
			selectedGameObject = &gameObject;
		}
	}
	else
	{
		nodeFlag |= ImGuiTreeNodeFlags_Selected * (selectedGameObject == &gameObject);
		bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)idElem, nodeFlag, gameObject.getName().c_str());

		if (selectedGameObject == &gameObject)
			nodeFlag |= ImGuiTreeNodeFlags_Selected;

		if (ImGui::IsItemClicked())
		{
			selectedGameObject = &gameObject;
		}

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

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		ImGui::OpenPopupOnItemClick("SceneGraphContext");

	if (ImGui::BeginPopup("SceneGraphContext"))
	{
		if (ImGui::MenuItem("Remove one", NULL, false)) {}
		if (ImGui::MenuItem("Remove all", NULL, false)) {}
		ImGui::EndPopup();
	}
}