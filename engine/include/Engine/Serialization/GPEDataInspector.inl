namespace GPE
{
template <typename T>
bool DataInspector::inspect(GPE::InspectContext& context, Linker<T>& inspected, const rfk::Field& info)
{
    return DataInspector::inspect(context, inspected, info.name.c_str());
}

template <typename T>
bool DataInspector::inspect(GPE::InspectContext& context, Linker<T>& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = false;

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
            hasChanged      = true;
        }

        ImGui::EndPopup();
    }

    // Drop from scene graph
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_GAMEOBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GPE::GameObject*));
            GPE::GameObject* pGo = *static_cast<GPE::GameObject**>(payload->Data);
            if (T* pCom = pGo->getComponent<T>())
            {
                inspected.pData = pComp;
                hasChanged      = true;
            }
        }
        ImGui::EndDragDropTarget();
    }
    endProperty();

    return hasChanged;
}

template <typename T>
void DataInspector::inspect(InspectContext& context, Linker<T>& inspected)
{
    DataInspector::inspect(context, inspected, "Component");
}
} // namespace GPE