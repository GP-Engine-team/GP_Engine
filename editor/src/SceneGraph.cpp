#include "Editor/SceneGraph.hpp"

// Components
// TODO: Generat this automatically
#include "Engine/ECS/Component/Camera.hpp"
#include "Engine/ECS/Component/Model.hpp"

#include "Engine/ECS/Component/AudioComponent.hpp"
#include "Engine/ECS/Component/InputComponent.hpp"

#include "Engine/ECS/Component/Light/DirectionalLight.hpp"
#include "Engine/ECS/Component/Light/PointLight.hpp"
#include "Engine/ECS/Component/Light/SpotLight.hpp"

#include "Engine/Resources/Scene.hpp"
#include <imgui/imgui.h>

using namespace Editor;
using namespace GPE;

void DeferedSetParent::bind(GameObject& movedGO, GameObject& newParentGO)
{
    m_movedGO     = &movedGO;
    m_newParentGO = &newParentGO;
}

void DeferedSetParent::tryExecute()
{
    if (m_movedGO == nullptr)
        return;

    m_movedGO->setParent(*m_newParentGO);

    m_movedGO     = nullptr;
    m_newParentGO = nullptr;
}

void SceneGraph::controlPreviousItem(GPE::GameObject& gameObject, GameObject*& selectedGameObject, int idElem)
{
    // Drag
    if (ImGui::BeginDragDropSource())
    {
        GPE::GameObject* pGameObject = &gameObject;
        ImGui::SetDragDropPayload("_GAMEOBJECT", &pGameObject, sizeof(pGameObject));
        ImGui::TextUnformatted(gameObject.getName().c_str());
        ImGui::EndDragDropSource();
    }

    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(&gameObject));
            deferedSetParent.bind(**static_cast<GPE::GameObject**>(payload->Data), gameObject);
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        selectedGameObject = &gameObject;
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup((std::string("SceneGraphContext") + std::to_string(idElem)).c_str());
    }

    if (ImGui::BeginPopup((std::string("SceneGraphContext") + std::to_string(idElem)).c_str()))
    {
        ImGui::Text(gameObject.getName().c_str());
        if (ImGui::MenuItem("Add child", NULL, false))
        {
            gameObject.addChild(GameObject::CreateArg{"NewGameObject"});
        }

        if (ImGui::BeginMenu("Add component"))
        {
            if (ImGui::MenuItem("Camera"))
                gameObject.addComponent<Camera>();

            if (ImGui::MenuItem("Model"))
                gameObject.addComponent<Model>();

            // if (ImGui::MenuItem("Audio component"))
            // gameObject.addComponent<AudioComponent>();

            if (ImGui::MenuItem("Input component"))
                gameObject.addComponent<InputComponent>();

            if (ImGui::BeginMenu("Lights"))
            {
                if (ImGui::MenuItem("Dircetional light"))
                    gameObject.addComponent<DirectionalLight>();

                if (ImGui::MenuItem("PointLight"))
                    gameObject.addComponent<PointLight>();

                if (ImGui::MenuItem("Spot light"))
                    gameObject.addComponent<SpotLight>();

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Remove", NULL, false))
        {
            selectedGameObject = nullptr;
            gameObject.destroy();
        }

        ImGui::EndPopup();
    }
}

void SceneGraph::recursiveSceneGraphNode(GPE::GameObject& gameObject, GameObject*& selectedGameObject, int idElem)
{
    ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_Selected * (selectedGameObject == &gameObject) |
                                  ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                  ImGuiTreeNodeFlags_SpanAvailWidth;

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
                recursiveSceneGraphNode(*child, selectedGameObject, ++idElem);
            }
            ImGui::TreePop();
        }
    }
}

void SceneGraph::renderAndGetSelected(GPE::GameObject& gameObject, GameObject*& selectedGameObject)
{
    recursiveSceneGraphNode(gameObject, selectedGameObject);
    deferedSetParent.tryExecute();
}