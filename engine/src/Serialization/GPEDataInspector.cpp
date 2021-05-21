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

            if (SharedPrefab* pSPref = Engine::getInstance()->resourceManager.get<SharedPrefab>(path.string().c_str()))
            {
                inspected = &pSPref->pref;
                pSPref->instanceCounter++;
            }
            else
            {
                SharedPrefab& sPref = Engine::getInstance()->resourceManager.add<SharedPrefab>(path.string().c_str(),
                                                                                               path.string().c_str());
                inspected           = &sPref.pref;
            }

            hasChanged = true;
        }
        ImGui::EndDragDropTarget();
    }
    endProperty();

    return hasChanged;
}