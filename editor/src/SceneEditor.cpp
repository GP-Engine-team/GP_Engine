#include <Editor/SceneEditor.hpp>

#include <Editor/Editor.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

#include <glfw/glfw3.h>


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


static unsigned int pickColor(unsigned short flag, unsigned short bitset)
{
    const unsigned int textColors[2]
    {
        ImGui::GetColorU32(ImGuiCol_TextDisabled),
        ImGui::GetColorU32(ImGuiCol_Text)
    };

    return textColors[(bool)(bitset & flag)];
}


void SceneEditor::renderGizmoControlBar()
{
    if (ImGui::BeginMenuBar())
    {
        float cursorX = ImGui::GetCursorPosX() + ImGui::GetColumnWidth();
        unsigned int col;

        // Active transformation operation
        for (unsigned char i = 0u; i < 3u; ++i)
        {
            cursorX -= ImGui::CalcTextSize(m_operations[i].string).x + 2.f * ImGui::GetStyle().ItemSpacing.x;
            col      = pickColor(ImGuizmo::OPERATION(m_operations[i].id), activeOperation);

            ImGui::SetCursorPosX(cursorX);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::MenuItem(m_operations[i].string);
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                activeOperation = ImGuizmo::OPERATION(m_operations[i].id ^ (activeOperation & m_operations[i].id));
            }
        }

        cursorX -= ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetCursorPosX(cursorX);
        ImGui::Separator();

        // Active referential
        for (unsigned char i = 0u; i < 2u; ++i)
        {
            cursorX -= ImGui::CalcTextSize(m_modes[i].string).x + 2.f * ImGui::GetStyle().ItemSpacing.x;
            col = pickColor(ImGuizmo::MODE(m_modes[i].id), activeMode);

            ImGui::SetCursorPosX(cursorX);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::MenuItem(m_modes[i].string);
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                activeMode = ImGuizmo::MODE(m_modes[i].id);
            }
        }

        ImGui::EndMenuBar();
    }
}


void SceneEditor::renderGizmo(float* inspectedTransfo)
{

    ImVec2 topLeft = ImGui::GetWindowPos();
    topLeft.y     += ImGui::GetWindowContentRegionMin().y;

    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();
   
    ImGuizmo::SetRect(topLeft.x, topLeft.y, view.width, view.height);

    ImGuizmo::Manipulate(view.camera.getView().e,
                         view.camera.getProjection().e,
                         activeOperation,
                         activeMode,
                         inspectedTransfo);
}


void SceneEditor::checkKeys()
{
    if (!view.capturingInputs() && !ImGuizmo::IsUsing())
    {
        GLFWwindow* const window = GPE::Engine::getInstance()->window.getGLFWWindow();

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::TRANSLATE;
        }

        else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::ROTATE;
        }

        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::SCALE;
        }
    }
}


void SceneEditor::checkCursor(GPE::GameObject*& inspectedObject)
{
    { // Check whether the cursor is inside the window
        ImVec2 topLeft = ImGui::GetWindowPos();
        topLeft.y     += ImGui::GetWindowContentRegionMin().y;

        const ImVec2 bottomRight{topLeft.x + view.width, topLeft.y + view.height};

        if (!ImGui::IsMouseHoveringRect(topLeft, bottomRight))
        {
            return;
        }
    }

    // Actual method content
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        captureInputs(true);
    }

    else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        captureInputs(false);
    }

    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)
             && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
             && !ImGuizmo::IsOver())
    {
        const unsigned int idSelectedGameObject = view.getHoveredGameObjectID();
        if (idSelectedGameObject)
        {
            if (GameObject* const selectedObject =
                view.pScene->getWorld().getGameObjectCorrespondingToID(idSelectedGameObject))
            {
                inspectedObject = selectedObject;
                ImGuizmo::Enable(true);
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
            ImGuizmo::Enable(false);
        }
    }

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && inspectedObject)
    {
        view.lookAtObject(*reinterpret_cast<GameObject*>(inspectedObject));
    }
}

// ========================== Public methods ==========================
SceneEditor::SceneEditor(GPE::Scene& scene)
    : view{scene},
      m_operations
      {
          {"Scale",     ImGuizmo::SCALE},
          {"Rotate",    ImGuizmo::ROTATE},
          {"Translate", ImGuizmo::TRANSLATE}
      },
      m_modes
      {
          {"World", ImGuizmo::WORLD},
          {"Local", ImGuizmo::LOCAL}
      },
      activeOperation{ImGuizmo::OPERATION::TRANSLATE},
      activeMode     {ImGuizmo::MODE::WORLD}
{
    ImGuizmo::AllowAxisFlip(false);
}

void SceneEditor::render(GPE::GameObject*& inspected)
{
    // Use the whole window rectangle
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    if (ImGui::Begin("Scene editor", nullptr, ImGuiWindowFlags_MenuBar))
    {
        checkCursor(inspected);

        renderGizmoControlBar();

        const ImVec2 size{ImGui::GetContentRegionAvail()};
        view.resize(int(size.x), int(size.y));
        view.render();

        ImGui::Image((void*)(intptr_t)view.textureID, size, {.0f, 1.f}, {1.f, .0f});

        if (inspected)
        {
            checkKeys();
            renderGizmo(inspected->getTransform().get().model.e);
        }
        
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
