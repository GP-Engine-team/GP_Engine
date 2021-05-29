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
//
// template <>
// bool GPE::DataInspector::inspect(GPE::InspectContext& context, Linker<GameObject>& inspected, const rfk::Field& info)
//{
//    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
//}
//
// template <>
// bool GPE::DataInspector::inspect(GPE::InspectContext& context, Linker<GameObject>& inspected, const char* name)
//{
//    startProperty(name);
//    bool hasChanged = false;
//
//    ImGui::Selectable(inspected.pData == nullptr ? "None" : inspected.pData->getName().c_str());
//
//    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
//    {
//        ImGui::OpenPopup("GOSelection");
//    }
//
//    if (ImGui::BeginPopup("GOSelection"))
//    {
//        if (ImGui::MenuItem("Remove", NULL, false))
//        {
//            inspected.pData = nullptr;
//            hasChanged      = true;
//        }
//
//        ImGui::EndPopup();
//    }
//
//    // Drop from scene graph
//    if (ImGui::BeginDragDropTarget())
//    {
//        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
//        {
//            IM_ASSERT(payload->DataSize == sizeof(GPE::GameObject*));
//            inspected.pData = *static_cast<GPE::GameObject**>(payload->Data);
//            hasChanged      = true;
//        }
//        ImGui::EndDragDropTarget();
//    }
//    endProperty();
//
//    return hasChanged;
//}
//
// template <>
// void GPE::DataInspector::inspect(InspectContext& context, Linker<GameObject>& inspected)
//{
//    GPE::DataInspector::inspect(context, inspected, "GameObject");
//}

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
                // Add the new reference
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
bool renderResourceExplorer(const char* name, T*& inRes, const char* acceptedPayload,
                            std::function<T*(const char*)> importer)
{
    bool  hasChanged        = false;
    auto& resourceContainer = GPE::Engine::getInstance()->resourceManager.getAll<T>();

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
        for (auto&& it = resourceContainer.begin(); &it->second != inRes; ++itemCurrent, ++it)
            ;
    }

    const char* label = inRes ? items[itemCurrent] : "None";
    if (ImGui::BeginCombo(name, label, 0))
    {
        static int isInContentBrowser = 0;
        ImGui::RadioButton("Used", &isInContentBrowser, 0);
        ImGui::SameLine();
        ImGui::RadioButton("All", &isInContentBrowser, 1);

        if (isInContentBrowser)
        {
            std::vector<GPE::ResourcesPath>& pathContainer =
                *GPE::Engine::getInstance()->resourceManager.get<std::vector<GPE::ResourcesPath>>("Path");

            for (auto&& path : pathContainer)
            {
                if (path.path.extension().string() == acceptedPayload)
                {
                    if (ImGui::Selectable(path.path.string().c_str()))
                    {
                        inRes      = importer(path.path.string().c_str());
                        hasChanged = true;
                    }
                }
            }
        }
        else
        {
            for (int n = 0; n < items.size(); n++)
            {
                const bool is_selected = (itemCurrent == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    itemCurrent = n;

                    auto&& it = resourceContainer.begin();
                    for (int i = 0; i < itemCurrent; ++i, ++it)
                        ;

                    inRes      = &it->second;
                    hasChanged = true;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(acceptedPayload))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

            if (T* pMesh = Engine::getInstance()->resourceManager.get<T>(path.string().c_str()))
            {
                inRes      = pMesh;
                hasChanged = true;
            }
            else
            {
                inRes      = importer(path.string().c_str());
                hasChanged = true;
            }
        }
    }

    return hasChanged;
}

template <>
bool DataInspector::inspect(InspectContext& context, class Material*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(InspectContext& context, class Material*& inspected, const char* name)
{
    return renderResourceExplorer<Material>("Material", inspected, ENGINE_MATERIAL_EXTENSION, loadMaterialFile);
}

template <>
bool DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const char* name)
{
    return renderResourceExplorer<Mesh>("Mesh", inspected, ENGINE_MESH_EXTENSION, loadMeshFile);
}

template <>
bool DataInspector::inspect(InspectContext& context, class Texture*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(InspectContext& context, class Texture*& inspected, const char* name)
{
    return renderResourceExplorer<Texture>("Texture", inspected, ENGINE_TEXTURE_EXTENSION, loadTextureFile);
}

template <>
bool DataInspector::inspect(InspectContext& context, class Shader*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(InspectContext& context, class Shader*& inspected, const char* name)
{
    return renderResourceExplorer<Shader>("Shader", inspected, ENGINE_SHADER_EXTENSION, loadShaderFile);
}