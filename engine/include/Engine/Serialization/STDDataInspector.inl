#include <Engine/Serialization/STDDataInspector.hpp>

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::vector<T>& inspected, const rfk::Field& info)
{
    return inspect(context, inspected, info.name.c_str());
}

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::vector<T>& inspected, const char* name)
{
    ImGui::PushID(&inspected);

    const ImGuiTreeNodeFlags nodeFlag =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    const bool arrayIsOpen = ImGui::TreeNodeEx((void*)name, nodeFlag, name);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup(std::string("VectorOption").c_str());
    }

    if (ImGui::BeginPopup(std::string("VectorOption").c_str()))
    {
        if (ImGui::MenuItem("Emplace back"))
        {
            if (inspected.empty())
            {
                inspected.emplace_back(); // TODO: if constexpr if T if arythmetique set 0
            }
            else
            {
                inspected.emplace_back(inspected.back());
            }
        }

        ImGui::EndPopup();
    }

    if (arrayIsOpen)
    {
        size_t i = 0;
        for (auto&& it = inspected.begin(); it != inspected.end(); ++i)
        {
            ImGui::PushID(&*it);
            const bool treeIsOpen =
                ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlag, std::string("Element " + std::to_string(i)).c_str());

            // Check if user want to remove current element
            bool isDestroy = false;
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(std::string("InternalVectorOption").c_str());
            }

            if (ImGui::BeginPopup(std::string("InternalVectorOption").c_str()))
            {
                if (ImGui::MenuItem("Remove"))
                {
                    it        = inspected.erase(it);
                    isDestroy = true;
                    --i;
                }

                ImGui::EndPopup();
            }

            if (!isDestroy)
            {
                // Check if user inspect the current element
                if (treeIsOpen)
                {
                    inspect(context, *it);
                }
                ++it;
            }

            if (treeIsOpen)
                ImGui::TreePop();

            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
    return false;
}

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected, const rfk::Field& info)
{
    return inspect(context, inspected, info.name.c_str());
}

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected, const char* name)
{
    if (inspected.get())
    {
        return inspect(context, *inspected.get(), name);
    }
    return false;
}

template <typename T>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::unique_ptr<T>& inspected)
{
    if (inspected.get())
    {
        inspect(context, *inspected.get());
    }
}

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected, const rfk::Field& info)
{
    return inspect(context, inspected, info.name.c_str());
}

template <typename T>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected, const char* name)
{
    if (inspected.get())
    {
        return inspect(context, *inspected.get(), name);
    }
    return false;
}

template <typename T>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::shared_ptr<T>& inspected)
{
    if (inspected.get())
    {
        inspect(context, *inspected.get());
    }
}
