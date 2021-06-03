#include "Engine/Serialization/GPMDataInspector.hpp"
#include "Engine/Serialization/Slider.hpp"
#include "GPM/Transform.hpp"
#include "GPM/Vector3.hpp"
#include "GPM/Vector4.hpp"
#include "imgui.h"

#include <windows.h>

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::SplitTransform& inspected, const char* name)
{
    DataInspector::inspect(context, inspected.position, "Position");
    DataInspector::inspect(context, inspected.scale, "Scale");
    DataInspector::inspect(context, inspected.rotation, "Rotation");
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector2& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        context.setDirty(ImGui::SliderFloat2(info.name.c_str(), inspected.e, property->min, property->max));
    }
    else
    {
        DataInspector::inspect(context, inspected, info.name.c_str());
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector2& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::DragFloat2("", inspected.e, 0.1));
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector2& inspected)
{
    ImGui::DragFloat2("", inspected.e, 0.1);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector3& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        context.setDirty(ImGui::SliderFloat3(info.name.c_str(), inspected.e, property->min, property->max));
    }
    else
    {
        DataInspector::inspect(context, inspected, info.name.c_str());
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector3& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::DragFloat3("", inspected.e, 0.1));
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector3& inspected)
{
    ImGui::DragFloat3("", inspected.e);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector4& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        context.setDirty(ImGui::SliderFloat4(info.name.c_str(), inspected.e, property->min, property->max));
    }
    else
    {
        DataInspector::inspect(context, inspected, info.name.c_str());
    }
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector4& inspected, const char* name)
{
    context.startProperty(name);
    context.setDirty(ImGui::DragFloat4("", inspected.e));
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
    context.endProperty();
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Vector4& inspected)
{
    ImGui::DragFloat4("", inspected.e);
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Quaternion& inspected, const char* name)
{
    GPM::Vec3 asRotation = inspected.eulerAngles() * 180.f / PI;                   // to degrees
    GPE::DataInspector::inspect(context, asRotation, name); // display as euler angles
    inspected            = GPM::Quaternion::fromEuler(asRotation * PI / 180.f);    // to radians
}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, GPM::Quaternion& inspected)
{
    GPM::Vec3 asRotation = inspected.eulerAngles() * 180.f / PI;     // to degrees
    GPE::DataInspector::inspect(context, asRotation);                // display as euler angles
    inspected = GPM::Quaternion::fromEuler(asRotation * PI / 180.f); // to radians
}