#include "Engine/Serialization/GPEDataInspector.hpp"

#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <filesystem>

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

    // Drop from content browser
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ENGINE_PREFAB_EXTENSION))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path& path = *static_cast<std::filesystem::path*>(payload->Data);

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