#include <Engine/Intermediate/GameObject.hpp>
#include <Engine/Resources/Linker.hpp>
#include <Engine/Serialization/Inspect.hpp>
#include <Engine/Serialization/xml/xmlSaver.hpp>

using namespace GPE;

void Linker<GameObject>::setData(GameObject& newData)
{
    pData = &newData;
}

void Linker<GameObject>::save(XmlSaver& context) const
{
    std::string str = "";
    if (pData)
    {
        str = pData->getAbsolutePath();
        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
    }

    GPE::save(context, str, XmlSaver::SaveInfo{"GLinker", "GLinker", 0});
}

void Linker<GameObject>::inspect(InspectContext& context)
{
    DataInspector::startProperty("GameObject");
    // bool hasChanged = false;

    ImGui::Selectable(pData == nullptr ? "None" : pData->getName().c_str());

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
    {
        ImGui::OpenPopup("GOSelection");
    }

    if (ImGui::BeginPopup("GOSelection"))
    {
        if (ImGui::MenuItem("Remove", NULL, false))
        {
            pData = nullptr;
            // hasChanged      = true;
        }

        ImGui::EndPopup();
    }

    // Drop from scene graph
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GPE::GameObject*));
            pData = *static_cast<GPE::GameObject**>(payload->Data);
            // hasChanged      = true;
        }
        ImGui::EndDragDropTarget();
    }
    DataInspector::endProperty();

    // return hasChanged;
}