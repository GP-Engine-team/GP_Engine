#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

using namespace GPE;

void Linker<GameObject>::setData(GameObject& newData)
{
    pData = &newData;
}

inline void inspectGameObjectLinker(InspectContext& context, Linker<GameObject>& inspected, const char* name)
{
    context.startProperty(name);

    ImGui::Selectable(inspected.pData == nullptr ? "None" : inspected.pData->getName().c_str());

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        ImGui::OpenPopup("GOSelection");
    }

    if (ImGui::BeginPopup("GOSelection"))
    {
        if (ImGui::MenuItem("Remove", NULL, false))
        {
            inspected.pData = nullptr;
            context.setDirty();
        }

        ImGui::EndPopup();
    }

    // Drop from scene graph
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GPE::GameObject*));
            inspected.pData = *static_cast<GPE::GameObject**>(payload->Data);
            context.setDirty();
        }
        ImGui::EndDragDropTarget();
    }
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect<Linker<GameObject>>(GPE::InspectContext& context, Linker<GameObject>& inspected,
                                                     const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect<Linker<GameObject>>(InspectContext& context, Linker<GameObject>& inspected,
                                                     const char* name)
{
    inspectGameObjectLinker(context, inspected, name);
}

template <>
void GPE::DataInspector::inspect<Linker<GameObject>>(InspectContext& context, Linker<GameObject>& inspected)
{
    GPE::DataInspector::inspect(context, inspected, "GameObject");
}

template <>
void DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
void DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected, const char* name)
{
    inspectGameObjectLinker(context, inspected, name);
}

template <>
void DataInspector::inspect<GameObject>(InspectContext& context, Linker<GameObject>& inspected)
{
    GPE::DataInspector::inspect(context, inspected, "GameObject");
}

// template <typename T>
// void Linker<T>::save(XmlSaver& context) const
//{
//    std::string str = "";
//    if (pData)
//    {
//        str = pData->getOwner().getAbsolutePath();
//        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
//    }
//
//    GPE::save(context, str, XmlSaver::SaveInfo{"CLinker", "CLinker", 0});
//}

//
// void Linker<GameObject>::save(XmlSaver& context) const
//{
//    std::string str = "";
//    if (pData)
//    {
//        str = pData->getAbsolutePath();
//        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
//    }
//
//    GPE::save(context, str, XmlSaver::SaveInfo{"GLinker", "GLinker", 0});
//}
