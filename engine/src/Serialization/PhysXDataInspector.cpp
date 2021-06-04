#include <Engine/Core/Physics/Collisions/Collider.hpp>
#include <Engine/Serialization/PhysXDataInspector.hpp>
#include <characterkinematic/PxCapsuleController.h>

using namespace GPE;
/*template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxController*& inspected, const char* name)
{
    bool  changed = false;
    float test;
    changed |= DataInspector::inspect(context, test, "m_controller");
    return changed;
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxMaterial*& inspected, const char* name)
{
    bool changed = false;
    // b |= DataInspector::inspect(context, inspected.
    return changed;
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, physx::PxShape*& inspected, const char* name)
{
    bool changed = false;
    // b |= DataInspector::inspect(context, inspected.
    return changed;
}*/

template <>
void DataInspector::inspect(GPE::InspectContext& context, Collider& inspected, const char* name)
{
    ImGui::Indent(16.0f);
    const bool isCollapsingHOpen = ImGui::CollapsingHeader("Collider");

    if (isCollapsingHOpen)
    {
        inspected.inspect(context);
    }
    ImGui::Unindent(16.0f);
}

template <>
void DataInspector::inspect(GPE::InspectContext& context, EShapeType& inspected, const char* name)
{
    context.startProperty(name);

    const char* items[] = {"E_SPHERE", "E_BOX"};

    int         value        = static_cast<int>(inspected);
    const char* item_current = items[static_cast<int>(inspected)]; //"E_SPHERE";

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
                context.setDirty();
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard
            }                                 // navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }
    context.endProperty();
}
