#include <Editor/SceneEditor.hpp>

#include <Editor/Editor.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>

#include <Engine/Resources/Script/FreeFly.hpp>

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
    const unsigned int textColors[2]{ImGui::GetColorU32(ImGuiCol_TextDisabled), ImGui::GetColorU32(ImGuiCol_Text)};

    return textColors[(bool)(bitset & flag)];
}

void SceneEditor::renderControlBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Frustum culling", nullptr, &view.drawFrustumScene);
            ImGui::MenuItem("Debug physic", nullptr, &view.drawDebugPhysic);
            ImGui::MenuItem("Debug Shape", nullptr, &view.drawDebugShape);
            ImGui::MenuItem("Debug Line", nullptr, &view.drawDebugLine);

            ImGui::EndMenu();
        }
        ImGui::Separator();

        float sliderWidth = (ImGui::GetContentRegionAvailWidth() - getGizmoControlBarWidth()) / 2.f;
        if (sliderWidth > 0)
        {
            float ts = Engine::getInstance()->timeSystem.getTimeScale();
            ImGui::TextUnformatted("Time scale");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(sliderWidth);
            if (ImGui::SliderFloat("##TimeScaleSlider", &ts, 0.f, 1.f))
            {
                Engine::getInstance()->timeSystem.setTimeScale(ts);
            }

            ImGui::Separator();

            ImGui::TextUnformatted("Speed");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(sliderWidth);
            ImGui::SliderFloat("##SpeedSlider", &view.freeFly.m_speed, 0.f, 100.f);
            ImGui::Separator();
        }

        renderGizmoControlBar();
        ImGui::EndMenuBar();
    }
}

float SceneEditor::getGizmoControlBarWidth()
{
    float width = 0.f;

    // Active transformation operation
    for (unsigned char i = 0u; i < IM_ARRAYSIZE(m_operations); ++i)
    {
        width += ImGui::CalcTextSize(m_operations[i].string).x + 2.f * ImGui::GetStyle().ItemSpacing.x;
    }
    width += ImGui::GetStyle().ItemSpacing.x;

    // Active referential
    for (unsigned char i = 0u; i < IM_ARRAYSIZE(m_modes); ++i)
    {
        width += ImGui::CalcTextSize(m_modes[i].string).x + 2.f * ImGui::GetStyle().ItemSpacing.x;
    }
    // brut value represente safe area and space of ImGui
    return width + 175;
}

void SceneEditor::renderGizmoControlBar()
{
    float        cursorX = ImGui::GetCursorPosX() + ImGui::GetColumnWidth();
    unsigned int col;

    // Active transformation operation
    for (unsigned char i = 0u; i < IM_ARRAYSIZE(m_operations); ++i)
    {
        cursorX -= ImGui::CalcTextSize(m_operations[i].string).x + 2.f * ImGui::GetStyle().ItemSpacing.x;
        col = pickColor(ImGuizmo::OPERATION(m_operations[i].id), activeOperation);

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
    for (unsigned char i = 0u; i < IM_ARRAYSIZE(m_modes); ++i)
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
}

void SceneEditor::renderGizmo(TransformComponent& transfo)
{
    ImVec2 topLeft = ImGui::GetWindowPos();
    topLeft.y += ImGui::GetWindowContentRegionMin().y;

    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();

    ImGuizmo::SetRect(topLeft.x, topLeft.y, float(view.width), float(view.height));

    const ImGuizmo::OPERATION operation = ImGuizmo::Manipulate(view.camera.getView().e, view.camera.getProjection().e,
                                                               activeOperation, activeMode, transfo.get().model.e);
    if (operation)
    {
        if (operation & ImGuizmo::SCALE)
        {
            transfo.setScale(transfo.get().scaling());
        }

        else if (operation & ImGuizmo::ROTATE)
        {
            transfo.setRotation(GPM::toQuaternion(transfo.get().rotation()));
        }

        else if (operation & ImGuizmo::TRANSLATE)
        {
            transfo.setTranslation(transfo.get().translation());
        }

        // The update is handled internally to prevent unecessary extraneous computations
        transfo.setDirty(false);
        transfo.OnUpdate();

        // Cascade the changes made on this transform
        for (GPE::GameObject* child : transfo.getOwner().children)
        {
            child->forceUpdate();
        }
    }
}

void SceneEditor::checkKeys()
{
    if (!view.capturingInputs() && !ImGuizmo::IsUsing())
    {
        GLFWwindow* const window = GPE::Engine::getInstance()->window.getGLFWWindow();

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            activeMode = ImGuizmo::LOCAL;
        }

        else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            activeMode = ImGuizmo::WORLD;
        }

        else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::TRANSLATE;
        }

        else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::ROTATE;
        }

        else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            activeOperation = ImGuizmo::SCALE;
        }
    }
}

void SceneEditor::checkCursor(GPE::GameObject*& inspectedObject)
{
    { // Check whether the cursor is inside the window
        ImVec2 topLeft = ImGui::GetWindowPos();
        topLeft.y += ImGui::GetWindowContentRegionMin().y;

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

    else if (!ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
             !ImGuizmo::IsOver())
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

void SceneEditor::dragDropLevelEditor(ReloadableCpp* cpp)
{
    // Drop prefab to instanciate world in front of the camera
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_PREFAB_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            GameObject* go = nullptr;
            GameObject& world{view.pScene->getWorld()};

            if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.string().c_str()))
            {
                const auto loadFunc = GET_PROCESS((*cpp), clonePrefab);
                go                  = loadFunc(pSPref->pref, world);
            }

            else
            {
                const auto loadFunc = GET_PROCESS((*cpp), loadPrefabFromPath);
                go                  = loadFunc(world, path.string().c_str());
            }

            if (go)
            {
                const TransformComponent& const camTransfo{view.cameraOwner->getTransform()};
                go->getTransform().setTranslation(camTransfo.getGlobalPosition() +
                                                  camTransfo.getVectorForward() * 10.f);
            }
        }
        ImGui::EndDragDropTarget();
    }
}

// ========================== Public methods ==========================
SceneEditor::SceneEditor(GPE::Scene& scene)
    : view(scene), m_operations{{"Scale (5)", ImGuizmo::SCALE},
                                {"Rotate (4)", ImGuizmo::ROTATE},
                                {"Translate (3)", ImGuizmo::TRANSLATE}},
      m_modes{{"World (2)", ImGuizmo::WORLD}, {"Local (1)", ImGuizmo::LOCAL}},
      activeOperation{ImGuizmo::OPERATION::TRANSLATE}, activeMode{ImGuizmo::MODE::WORLD}
{
    ImGuizmo::AllowAxisFlip(false);
}

void SceneEditor::render(Editor& editorContext)
{
    // Use the whole window rectangle
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {.0f, .0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, .0f);

    GameObject*             inspected = dynamic_cast<GameObject*>(editorContext.inspectedObject);
    const GameObject* const prev      = inspected;

    if (ImGui::Begin("Scene editor", nullptr, ImGuiWindowFlags_MenuBar))
    {
        checkCursor(inspected);

        renderControlBar();

        const ImVec2 size{ImGui::GetContentRegionAvail()};
        view.resize(int(size.x), int(size.y));
        view.render();

        ImGui::Image((void*)(intptr_t)view.textureID, size, {.0f, 1.f}, {1.f, .0f});

        if (inspected)
        {
            checkKeys();
            renderGizmo(inspected->getTransform());
        }

        dragDropLevelEditor(editorContext.reloadableCpp);
    }

    ImGui::End();
    ImGui::PopStyleVar(2);

    if (prev != inspected)
    {
        editorContext.inspectedObject = inspected;
    }
}

} // End of namespace Editor
