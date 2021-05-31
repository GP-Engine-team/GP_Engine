#include <Editor/SceneGraph.hpp>

#include <Editor/Editor.hpp>

#include <Engine/Core/HotReload/ReloadableCpp.hpp>
#include <Engine/Core/HotReload/SingletonsSync.hpp>
#include <Engine/Core/Tools/Hash.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Scene.hpp>
#include <Engine/Serialization/FileExplorer.hpp>
#include <Engine/Serialization/IInspectable.hpp>

#include <filesystem>
#include <imgui/imgui.h>
//#include <map>

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

    m_movedGO->setParent(m_newParentGO);

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

            GameObject* go = nullptr;
            if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.string().c_str()))
            {
                auto loadFunc = GET_PROCESS((*m_pEditorContext->reloadableCpp), clonePrefab);
                go            = loadFunc(pSPref->pref, gameObject);
            }
            else
            {
                auto loadFunc = GET_PROCESS((*m_pEditorContext->reloadableCpp), loadPrefabFromPath);
                go            = loadFunc(gameObject, path.string().c_str());
            }
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
            auto getComponentClassFunct   = GET_PROCESS((*m_pEditorContext->reloadableCpp), getComponentClass);
            auto createComponentByIDFunct = GET_PROCESS((*m_pEditorContext->reloadableCpp), createComponentByID);

            std::map<std::string, const rfk::Struct*> compSortedByName;

            for (auto&& child : getComponentClassFunct().children)
            {
                compSortedByName[child->name] = child;
            }

            for (const auto& [key, value] : compSortedByName)
            {
                if (ImGui::MenuItem(key.c_str()))
                    createComponentByIDFunct(gameObject, value->id);
            }

            // Sorting mus be apply when RFK will be update with directChildren function
            // std::vector<std::map<std::string, std::vector<rfk::Archetype const*>>> archetypeSortedByDepth;
            // for (auto& parent : classArchetype->directParents)
            //{
            //    if (!parent.type->directParents.empty())
            //    {
            //        archetypeSorted[parent.type->name].emplace_back(compArchetype);
            //    }
            //    else
            //    {
            //        archetypeSorted[compArchetype->name].emplace_back(compArchetype);
            //    }
            //}

            // std::vector < std::map<std::string, std::vector<rfk::Archetype const*>> archetypeSortedByDepth;

            // for (auto&& compArchetype : componnentList)
            //{
            //    if (ImGui::MenuItem(key.c_str()))
            //        createComponentByIDFunct(gameObject, value.front()->id);
            //}

            // std::map<std::string, std::vector<rfk::Archetype const*>> archetypeSorted;

            // for (auto&& compArchetype : componnentList)
            //{
            //    const rfk::Class* classArchetype = static_cast<const rfk::Class*>(compArchetype);
            //    for (auto& parent : classArchetype->directParents)
            //    {
            //        if (!parent.type->directParents.empty())
            //        {
            //            archetypeSorted[parent.type->name].emplace_back(compArchetype);
            //        }
            //        else
            //        {
            //            archetypeSorted[compArchetype->name].emplace_back(compArchetype);
            //        }
            //    }
            //}

            // for (const auto& [key, value] : archetypeSorted)
            //{
            //    if (value.size() > 1)
            //    {
            //        if (ImGui::BeginMenu(key.c_str()))
            //        {
            //            for (auto&& archetype : value)
            //            {
            //                if (ImGui::MenuItem(archetype->name.c_str()))
            //                    createComponentByIDFunct(gameObject, archetype->id);
            //            }
            //            ImGui::EndMenu();
            //        }
            //    }
            //    else
            //    {
            //        if (ImGui::MenuItem(key.c_str()))
            //            createComponentByIDFunct(gameObject, value.front()->id);
            //    }
            //}

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Save as prefab", NULL, false))
        {
            const std::filesystem::path path = openFolderExplorerAndGetRelativePath(L"Select folder") /
                                               (gameObject.getName() + ENGINE_PREFAB_EXTENSION);

            auto saveFunc = GET_PROCESS((*m_pEditorContext->reloadableCpp), savePrefabToPath);
            saveFunc(gameObject, path.string().c_str(), GPE::SavedScene::EType::XML);
        }

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

    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && selectedGameObject)
        m_pEditorContext->sceneEditor.view.lookAtObject(*static_cast<GameObject*>(selectedGameObject));
}

void SceneGraph::renderAndGetSelected(GPE::GameObject& gameObject, GPE::IInspectable*& selectedGameObject)
{
    recursiveSceneGraphNode(gameObject, selectedGameObject);
    deferedSetParent.tryExecute();
}