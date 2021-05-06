#include "Editor/SceneGraph.hpp"

#include <filesystem>

#include "Engine/Core/Tools/Hash.hpp"
#include <Editor/Editor.hpp>
#include <Engine/Engine.hpp>

// Components
// TODO: Generat this automatically
#include <Engine/ECS/Component/AudioComponent.hpp>
#include <Engine/ECS/Component/Camera.hpp>
#include <Engine/ECS/Component/InputComponent.hpp>
#include <Engine/ECS/Component/Light/DirectionalLight.hpp>
#include <Engine/ECS/Component/Light/PointLight.hpp>
#include <Engine/ECS/Component/Light/SpotLight.hpp>
#include <Engine/ECS/Component/Model.hpp>
#include <Engine/ECS/Component/ParticleComponent.hpp>

#include <Engine/Serialization/IInspectable.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Serialization/FileExplorer.hpp>

#include <Editor/ExternalDeclarations.hpp>
#include <Engine/Core/HotReload/ReloadableCpp.hpp>

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

SceneGraph::SceneGraph(Editor& context) : m_pEditorContext{&context}
{
}

void SceneGraph::controlPreviousItem(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject, int idElem)
{
    // Drag
    if (ImGui::BeginDragDropSource())
    {
        GPE::GameObject* pGameObject = &gameObject;
        ImGui::SetDragDropPayload("_GAMEOBJECT", &pGameObject, sizeof(pGameObject));
        ImGui::TextUnformatted(gameObject.getName().c_str());
        ImGui::EndDragDropSource();
    }

    // Drop gameObject to set as child
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(&gameObject));
            deferedSetParent.bind(**static_cast<GPE::GameObject**>(payload->Data), gameObject);
        }
        ImGui::EndDragDropTarget();
    }

    // Drop prefab to instanciate and set as child
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_PREFAB_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            Scene empty;
            auto  loadFunc = GET_PROCESS((*m_pEditorContext->m_reloadableCpp), loadSceneFromPath);
            loadFunc(&empty, path.string().c_str());
            empty.getWorld().setParent(gameObject);

            // if (Prefab* pPrefab = Engine::getInstance()->resourceManager.get<Prefab>(path.string().c_str()))
            //{
            //    inspected.pPrefab = pPrefab;
            //}
            // else
            //{
            //    if (const std::string* str = Engine::getInstance()->resourceManager.getKey(inspected.pPrefab))
            //        inspected.pModel->getOwner().pOwnerScene->removeLoadedResourcePath(str->c_str());

            //    inspected.pPrefab = loadPrefabFile(path.string().c_str());
            //    inspected.pModel->getOwner().pOwnerScene->addLoadedResourcePath(path.string().c_str());
            //}
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
    {
        selectedGameObject = &gameObject;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
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

            if (ImGui::MenuItem("Particle component"))
                gameObject.addComponent<ParticleComponent>();

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

        // if (ImGui::MenuItem("Save as prefab", NULL, false))
        //{
        //    const std::filesystem::path path = openFileExplorerAndGetRelativePath(L"Select folder", {});

        //    Scene tempScene(gameObject);
        //    gameObject.moveTowardScene(tempScene);
        //    auto saveFunc = GET_PROCESS((*m_pEditorContext.m_reloadableCpp), saveSceneToPath);
        //    saveFunc(tempScene, path, GPE::SavedScene::EType::XML);
        //}

        if (ImGui::MenuItem("Remove", NULL, false))
        {
            selectedGameObject = nullptr;
            gameObject.destroy();
        }

        ImGui::EndPopup();
    }
}

void SceneGraph::recursiveSceneGraphNode(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject,
                                         int idElem)
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

void SceneGraph::renderAndGetSelected(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject)
{
    recursiveSceneGraphNode(gameObject, selectedGameObject);
    deferedSetParent.tryExecute();
}