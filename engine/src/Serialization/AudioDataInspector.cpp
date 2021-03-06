﻿#include <imgui.h>

#include <Engine/ECS/Component/AudioComponent.hpp>

#include <Engine/Serialization/AudioDataInspector.hpp>

using namespace GPE;

/*template <>
bool DataInspector::inspect(GPE::InspectContext& context, Collider& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(GPE::InspectContext& context, Collider& inspected, const char* name)
{
    ImGui::Indent(16.0f);
    const bool hasChanged        = false;
    const bool isCollapsingHOpen = ImGui::CollapsingHeader("Collider");

    if (isCollapsingHOpen)
    {
        inspected.inspect(context);
    }
    ImGui::Unindent(16.0f);
    return hasChanged;
}
*/
/*template <>
bool DataInspector::inspect(GPE::InspectContext& context, EShapeType& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool DataInspector::inspect(GPE::InspectContext& context, EShapeType& inspected, const char* name)
{
    bool hasChanged = false;
    startProperty(name);

    const char*        items[]      = {"E_SPHERE", "E_BOX"};
    static const char* item_current = items[static_cast<int>(inspected)]; //"E_SPHERE";

    if (ImGui::BeginCombo("##ComboBoxCollider", item_current,
                          0)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (item_current == items[n]);

            if (ImGui::Selectable(items[n], is_selected))
            {
                item_current = items[n];
                inspected    = static_cast<GPE::EShapeType>(n);
                hasChanged   = true;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard
            }                                 // navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }
    endProperty();

    return hasChanged;
}
*/

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, std::unordered_map<std::string, SourceData>& inspected,
                                 const char* name)
{
    context.startProperty(name, 0.f);

    for (auto& [key, value] : inspected)
    {
        // ImGui::Text(value.name.c_str());
        if (ImGui::TreeNode(value.sourceName.c_str()))
        {
            value.settings.inspect(context);
            value.isDirty = context.isDirty();
            ImGui::TreePop();
        }
    }

    context.endProperty();
}
