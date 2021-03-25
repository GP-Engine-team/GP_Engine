#include "Engine/Serialization/GPMDataInspector.hpp"
#include "GPM/Transform.hpp"
#include "Engine/Serialization/Slider.hpp"
#include "imgui.h"

template <>
void GPE::DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
void GPE::DataInspector::inspect(GPM::SplitTransform& inspected, const char* name)
{
    DataInspector::inspect(inspected.position,  "Position");
    DataInspector::inspect(inspected.scale,     "Scale");
    DataInspector::inspect(inspected.rotation,  "Rotation");
}

template <>
void GPE::DataInspector::inspect(GPM::Vector3& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        ImGui::SliderFloat3(info.name.c_str(), inspected.e, property->min, property->max);
    }
    else
    {
        DataInspector::inspect(inspected, info.name.c_str());
    }
}

template <>
void GPE::DataInspector::inspect(GPM::Vector3& inspected, const char* name)
{
    startProperty(name);
    ImGui::DragFloat3("", inspected.e);
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
    endProperty();
}

template <>
void GPE::DataInspector::inspect(GPM::Quaternion& inspected, const rfk::Field& info)
{
    GPE::DataInspector::inspect(inspected, info.name.c_str());
}


template <>
void GPE::DataInspector::inspect(GPM::Quaternion& inspected, const char* name)
{
    GPM::Vec3 asRotation = inspected.eulerAngles() * 180.f / PI;     // to degrees
    GPE::DataInspector::inspect(asRotation, name);                   // display as euler angles
    inspected = GPM::Quaternion::fromEuler(asRotation * PI / 180.f); // to radians
}
