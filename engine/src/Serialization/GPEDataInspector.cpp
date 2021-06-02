
#include <filesystem>
#include <unordered_map>

#include <imgui.h>

#include <Engine/Engine.hpp>
#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Importer/Importer.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Resources/Prefab.hpp>
#include <Engine/Resources/ResourcesManager.hpp>
#include <Engine/Resources/Scene.hpp>

#include <Engine/Serialization/GPEDataInspector.hpp>

using namespace GPE;

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::ColorEdit3("", &inspected.e[0]));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGB& inspected)
{
    ImGui::ColorEdit3("", &inspected.e[0]);
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::ColorEdit4("", &inspected.e[0]));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected)
{
    ImGui::ColorEdit4("", &inspected.e[0]);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::ColorEdit4("", &inspected.e[0]));
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, Prefab*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, Prefab*& inspected, const char* name)
{
    context.startProperty(name);

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
            context.setDirty();
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

            context.setDirty();
        }
        ImGui::EndDragDropTarget();
    }
    context.endProperty();
}

template <typename T>
void renderResourceExplorer(InspectContext& context, const char* name, T*& inRes, const char* acceptedPayload,
                            std::function<T*(const char*)> importer)
{
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
                        context.setDirty();
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
                    context.setDirty();
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
                context.setDirty();
            }
            else
            {
                inRes      = importer(path.string().c_str());
                context.setDirty();
            }
        }
    }
}


template <typename T>
void renderAnimResourceExplorer(InspectContext& context, const char* name, T*& inRes, const char* acceptedPayload,
                                std::function<T*(const char*)> importer)
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
                        context.setDirty();
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
                    context.setDirty();
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

            if (T* pMesh = Engine::getInstance()->animResourcesManager.get<T>(path.string().c_str()))
            {
                inRes      = pMesh;
                context.setDirty();
            }
            else
            {
                inRes      = importer(path.string().c_str());
                context.setDirty();
            }
        }
    }
}

template <>
void GPE::DataInspector::inspect<Skin*>(InspectContext & context, GPE::Skin * &inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect(InspectContext& context, class Skin*& inspected, const char* name)
{
    renderAnimResourceExplorer<Skin>(context, "Skin", inspected, ENGINE_SKIN_EXTENSION, loadSkinFile);
}

template <>
void GPE::DataInspector::inspect<Skeleton*>(InspectContext & context, GPE::Skeleton*& inspected,
                                            const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect(InspectContext& context, class Material*& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, GPE::Skeleton*& inspected, const char* name)
{
    // Ressources should always be inspected from a component.
    assert(context.lastComponentOwner != nullptr);

    renderAnimResourceExplorer<Skeleton>(context, "Skeleton", inspected, ENGINE_SKELETON_EXTENSION, loadSkeletonFile);
}

template <>
void GPE::DataInspector::inspect<Animation*>(InspectContext& context, Animation*& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect(InspectContext& context, class Material*& inspected, const char* name)
{
    renderResourceExplorer<Material>(context, "Material", inspected, ENGINE_MATERIAL_EXTENSION, loadMaterialFile);
}

template <>
void DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(InspectContext & context, class Animation * &inspected, const char* name)
{
    // Ressources should always be inspected from a component.
    assert(context.lastComponentOwner != nullptr);

    renderAnimResourceExplorer<Animation>(context, "Animation", inspected, ENGINE_ANIMATION_EXTENSION,
                                          loadAnimationFile);
}

template <>
void DataInspector::inspect(InspectContext& context, class Mesh*& inspected, const char* name)
{
    renderResourceExplorer<Mesh>(context, "Mesh", inspected, ENGINE_MESH_EXTENSION, loadMeshFile);
}

template <>
void DataInspector::inspect(InspectContext& context, class Texture*& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect(InspectContext& context, class Texture*& inspected, const char* name)
{
    renderResourceExplorer<Texture>(context, "Texture", inspected, ENGINE_TEXTURE_EXTENSION, loadTextureFile);
}

template <>
void DataInspector::inspect(InspectContext& context, class Shader*& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect(InspectContext& context, class Shader*& inspected, const char* name)
{
    renderResourceExplorer<Shader>(context, "Shader", inspected, ENGINE_SHADER_EXTENSION, loadShaderFile);
}
