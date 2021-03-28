#include "Engine/Serialization/STDDataInspector.hpp"

template <typename T>
void GPE::DataInspector::inspect(std::vector<T>& inspected, const rfk::Field& info)
{
    ImGuiTreeNodeFlags nodeFlag =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (ImGui::TreeNodeEx((void*)info.name.c_str(), nodeFlag, info.name.c_str()))
    {
        for (size_t i = 0; i < inspected.size(); ++i)
        {
            if (ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlag, std::string("Element " + std::to_string(i)).c_str()))
            {
                inspect(inspected[i]);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}