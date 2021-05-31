#include "Engine/Serialization/GPEDataInspector.hpp"

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>
#include <Engine/Resources/Scene.hpp>
#include <filesystem>
#include <unordered_map>

#include <imgui.h>

using namespace GPE;

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit3("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGB& inspected)
{
    ImGui::ColorEdit3("", &inspected.e[0]);
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit4("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected)
{
    ImGui::ColorEdit4("", &inspected.e[0]);
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit4("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, GameObject*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, GameObject*& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = false;

    ImGui::Selectable(inspected == nullptr ? "None" : inspected->getName().c_str());

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        ImGui::OpenPopup("GOSelection");
    }

    if (ImGui::BeginPopup("GOSelection"))
    {
        if (ImGui::MenuItem("Remove", NULL, false))
        {
            inspected  = nullptr;
            hasChanged = true;
        }

        ImGui::EndPopup();
    }

    // Drop from scene graph
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GPE::GameObject*));
            inspected  = *static_cast<GPE::GameObject**>(payload->Data);
            hasChanged = true;
        }
        ImGui::EndDragDropTarget();
    }
    endProperty();

    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, Prefab*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, Prefab*& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = false;

    ImGui::Selectable(inspected ? inspected->getName() : "None");

    if (inspected && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        ImGui::OpenPopup("PrefabSelection");
    }

    if (ImGui::BeginPopup("PrefabSelection"))
    {
        if (ImGui::MenuItem("Remove", NULL, false))
        {
            std::unordered_map<std::string, SharedPrefab>& pSPref =
                Engine::getInstance()->resourceManager.getAll<GPE::SharedPrefab>();

            for (auto&& it = pSPref.begin(); it != pSPref.end(); ++it)
            {
                if (&it->second.pref == inspected)
                {
                    if (--it->second.instanceCounter == 0)
                        pSPref.erase(it);
                    break;
                }
            }

            inspected  = nullptr;
            hasChanged = true;
        }

        ImGui::EndPopup();
    }

    // Drop from content browser
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_PREFAB_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            // Remove previous reference of prefab
            if (inspected)
            {
                auto&& map = GPE::Engine::getInstance()->resourceManager.getAll<SharedPrefab>();
                for (auto&& it = map.begin(); it != map.end(); ++it)
                {
                    if (&it->second.pref == inspected)
                    {
                        if (--it->second.instanceCounter == 0)
                            map.erase(it);
                        break;
                    }
                }
            }

            if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.string().c_str()))
            {
                //Add the new reference
                inspected = &pSPref->pref;
                pSPref->instanceCounter++;
            }
            else
            {
                SharedPrefab& sPref = Engine::getInstance()->resourceManager.add<SharedPrefab>(path.string().c_str(),
                                                                                               path.string().c_str());
                inspected           = &sPref.pref;
                ++sPref.instanceCounter;
            }

            hasChanged = true;
        }
        ImGui::EndDragDropTarget();
    }
    endProperty();

    return hasChanged;
}

template <typename T>
void renderAnimResourceExplorer(const char* name, T*& inRes)
{
    auto& resourceContainer = GPE::Engine::getInstance()->animResourcesManager.getAll<T>();

    std::vector<const char*> items;
    items.reserve(resourceContainer.size());

    for (auto&& res : resourceContainer)
    {
        items.emplace_back(res.first.c_str());
    }

    // Init position of the combo box cursor
    int itemCurrent;
    if (inRes == nullptr)
    {
        itemCurrent = -1;
    }
    else
    {
        itemCurrent = 0;
        for (auto&& it = resourceContainer.begin(); (it != resourceContainer.end()) && (&it->second != inRes); ++itemCurrent, ++it)
            ;
    }

    if (ImGui::Combo(name, &itemCurrent, items.data(), static_cast<int>(items.size())))
    {
        auto&& it = resourceContainer.begin();
        for (int i = 0; i < itemCurrent; ++i, ++it)
            ;

        inRes = &it->second;
    }
}

//template <>
//bool GPE::DataInspector::inspect<Skin>(InspectContext& context, GPE::Skin*& inspected, const rfk::Field& info)
//{
//    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
//}

template <>
bool GPE::DataInspector::inspect<Skin*>(InspectContext& context, GPE::Skin*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, GPE::Skin*& inspected, const char* name)
{
    // Ressources should always be inspected from a component.
    assert(context.lastComponentOwner != nullptr);

    renderAnimResourceExplorer("Skin", inspected);
    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_SKIN_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (Skin* pSkin = Engine::getInstance()->animResourcesManager.get<Skin>(path.string().c_str()))
            {
                inspected = pSkin;
            }
            else
            {
                if (const std::string* str = Engine::getInstance()->animResourcesManager.getKey(inspected))
                    context.lastComponentOwner->pOwnerScene->removeLoadedResourcePath(str->c_str());

                inspected = loadSkinFile(path.string().c_str());
                context.lastComponentOwner->pOwnerScene->addLoadedResourcePath(path.string().c_str());
            }
        }
    }

    return true;
}

template <>
bool GPE::DataInspector::inspect<Skeleton*>(InspectContext& context, GPE::Skeleton*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, GPE::Skeleton*& inspected, const char* name)
{
    // Ressources should always be inspected from a component.
    assert(context.lastComponentOwner != nullptr);

    renderAnimResourceExplorer("Skeleton", inspected);
    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_SKELETON_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (Skeleton* pSkeleton = Engine::getInstance()->animResourcesManager.get<Skeleton>(path.string().c_str()))
            {
                inspected = pSkeleton;
            }
            else
            {
                if (const std::string* str = Engine::getInstance()->animResourcesManager.getKey(inspected))
                    context.lastComponentOwner->pOwnerScene->removeLoadedResourcePath(str->c_str());

                inspected = loadSkeletonFile(path.string().c_str());
                context.lastComponentOwner->pOwnerScene->addLoadedResourcePath(path.string().c_str());
            }
        }
    }

    return true;
}