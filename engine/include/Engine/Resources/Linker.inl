namespace GPE
{
template <typename T>
void Linker<T>::setData(GameObject& owner)
{
    pData = owner.getComponent<T>();
}

template <typename T>
void Linker<T>::save(XmlSaver& context) const
{
    std::string str = "";
    if (pData)
    {
        str = pData->getOwner().getAbsolutePath();
        str.erase(0, str.find_first_of('/', 0) + 1); // remove the world
    }

    GPE::save(context, str, XmlSaver::SaveInfo{"CLinker", "CLinker", 0});
}

template <typename T>
void Linker<T>::inspect(InspectContext& context)
{
    DataInspector::startProperty("Component");
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
            GPE::GameObject* pGo = *static_cast<GPE::GameObject**>(payload->Data);
            if (T* pCom = pGo->getComponent<T>())
            {
                pData = pCom;
                // hasChanged      = true;
            }
        }
        ImGui::EndDragDropTarget();
    }
    DataInspector::endProperty();

    // return hasChanged;
}
} // namespace GPE