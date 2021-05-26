#include <Editor/SceneEditor.hpp>

#include <Editor/Editor.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <imgui/imgui.h>

// Don't move up
#include "Engine/Core/HotReload/SingletonsSync.hpp"

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
SceneEditor::SceneEditor(Editor& editorContext, GPE::Scene& scene) : m_editorContext{&editorContext}, view{scene}
{
}

void SceneEditor::render(GPE::IInspectable*& inspectedObject)
{
    // Use the whole window content
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    const std::string windowName = "Scene editor (" + view.pScene->getName() + ')';
    if (ImGui::Begin(windowName.c_str()))
    {
        const ImVec2 size{ImGui::GetContentRegionAvail()};

        checkCursor(inspectedObject);

        view.resize(int(size.x), int(size.y));
        view.render();

        ImGui::Image((void*)(intptr_t)view.textureID, size, {.0f, 1.f}, {1.f, .0f});

        // Drop prefab to instanciate world in front of the camera
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_PREFAB_EXTENSION))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

                GameObject* go = nullptr;
                if (SharedPrefab* pSPref =
                        Engine::getInstance()->resourceManager.get<SharedPrefab>(path.string().c_str()))
                {
                    auto loadFunc = GET_PROCESS((*m_editorContext->m_reloadableCpp), clonePrefab);
                    go            = loadFunc(pSPref->pref, view.pScene->getWorld());
                }
                else
                {
                    auto loadFunc = GET_PROCESS((*m_editorContext->m_reloadableCpp), loadPrefabFromPath);
                    go            = loadFunc(view.pScene->getWorld(), path.string().c_str());
                }

                if (go)
                {
                    go->getTransform().setTranslation(view.cameraOwner->getTransform().getGlobalPosition() +
                                                      view.cameraOwner->getTransform().getVectorForward() * 10.f);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

} // End of namespace Editor
