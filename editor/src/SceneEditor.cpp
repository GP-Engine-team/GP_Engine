#include "Editor/SceneEditor.hpp"

#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Resources/Scene.hpp"
#include "imgui/imgui.h"

namespace Editor
{

using namespace GPE;

// ========================== Private methods ==========================
void SceneEditor::checkCursor(GPE::IInspectable*& inspectedObject)
{
    if (ImGui::IsMouseClicked(0))
    {
        const bool hovered = ImGui::IsWindowHovered();
        view.captureInputs(hovered);

        const unsigned int idSelectedGameObject = view.getHoveredGameObjectID();

        if (idSelectedGameObject && hovered)
        {
            if (GameObject* const selectionGameObject = view.pScene->getWorld()
                                                        .getGameObjectCorrespondingToID(idSelectedGameObject))

            {
                inspectedObject = selectionGameObject;
            }

            else
            {
                GPE::Log::getInstance()->logError(
                    stringFormat("No gameObject corresponding to the id %i", idSelectedGameObject));
            }
        }
    }
}




// ========================== Public methods ==========================
SceneEditor::SceneEditor(GPE::Scene& scene)
	: view{scene}
{}


void SceneEditor::render(GPE::IInspectable*& inspectedObject)
{
	// Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    ImGui::Begin(view.pScene->getWorld().getName().c_str(), nullptr, ImGuiWindowFlags_NoBackground);
    {
        checkCursor(inspectedObject);

        const ImVec2 size{ImGui::GetContentRegionAvail()};

        view.resize(static_cast<int>(size.x), static_cast<int>(size.y));
        view.render();

        ImGui::Image((void*)(intptr_t)view.textureID, size, {0.f, 1.f}, {1.f, 0.f});
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}

} // End of namespace Editor
