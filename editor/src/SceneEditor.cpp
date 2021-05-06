#include <Editor/SceneEditor.hpp>

#include <Engine/Engine.hpp>
#include <imgui/imgui.h>

namespace Editor
{

using namespace GPE;

// ========================== Private methods ==========================
void SceneEditor::captureInputs(bool toggle)
{
    InputManager& io = Engine::getInstance()->inputManager;
    io.setCursorLockState(toggle);
    io.setCursorTrackingState(toggle);
    view.captureInputs(toggle);

    if (toggle)
    {
        io.setInputMode("Level editor");
    }
    else
    {
        io.restorePreviousInputMode();
    }
}

void SceneEditor::checkCursor(GPE::IInspectable*& inspectedObject)
{
    const bool hovered = ImGui::IsWindowHovered();

    if (hovered)
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            captureInputs(true);
        }

        else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            captureInputs(false);
        }

        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            const unsigned int idSelectedGameObject = view.getHoveredGameObjectID();
            if (idSelectedGameObject)
            {
                if (GameObject* const selectionGameObject =
                        view.pScene->getWorld().getGameObjectCorrespondingToID(idSelectedGameObject))
                {
                    inspectedObject = selectionGameObject;
                }

                else
                {
                    GPE::Log::getInstance()->logError(
                        stringFormat("No gameObject corresponding to the id %i", idSelectedGameObject));
                }
            }

            else
            {
                inspectedObject = nullptr;
            }
        }
        
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && inspectedObject)
            view.lookAtObject(*reinterpret_cast<GameObject*>(inspectedObject));
    }
}

// ========================== Public methods ==========================
SceneEditor::SceneEditor(GPE::Scene& scene)
    : view{scene}
{
}

void SceneEditor::render(GPE::IInspectable*& inspectedObject)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    const std::string windowName = "Scene editor (" + view.pScene->getWorld().getName() + ')';
    if (ImGui::Begin(windowName.c_str()))
    {
        const ImVec2 size{ImGui::GetContentRegionAvail()};

        checkCursor(inspectedObject);

        view.resize(int(size.x), int(size.y));
        view.render();

        ImGui::Image((void*)(intptr_t)view.textureID, size, {.0f, 1.f}, {1.f, .0f});
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}

} // End of namespace Editor
