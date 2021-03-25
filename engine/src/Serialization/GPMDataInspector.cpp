#include "Engine/Serialization/GPMDataInspector.hpp"
#include "GPM/Transform.hpp"
#include "Engine/Serialization/Slider.hpp"
#include "imgui.h"

#include <windows.h>

template <>
bool GPE::DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPM::SplitTransform& inspected, const char* name)
{
    bool b = false;
    b |= DataInspector::inspect(inspected.position,  "Position");
    b |= DataInspector::inspect(inspected.scale, "Scale");
    b |= DataInspector::inspect(inspected.rotation, "Rotation");
    return b;
}

template <>
bool GPE::DataInspector::inspect(GPM::Vector3& inspected, const rfk::Field& info)
{
    Slider const* property = info.getProperty<Slider>();
    if (property)
    {
        return ImGui::SliderFloat3(info.name.c_str(), inspected.e, property->min, property->max);
    }
    else
    {
        return DataInspector::inspect(inspected, info.name.c_str());
    }
}

template <>
bool GPE::DataInspector::inspect(GPM::Vector3& inspected, const char* name)
{
    startProperty(name);
    bool hasChanged = ImGui::DragFloat3("", inspected.e);
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
    endProperty();
    return hasChanged;
}

template <>
bool GPE::DataInspector::inspect(GPM::Quaternion& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(inspected, info.name.c_str());
}


template <>
bool GPE::DataInspector::inspect(GPM::Quaternion& inspected, const char* name)
{
    GPM::Vec3 asRotation = inspected.eulerAngles() * 180.f / PI;     // to degrees
    bool hasChanged = GPE::DataInspector::inspect(asRotation, name); // display as euler angles
    inspected = GPM::Quaternion::fromEuler(asRotation * PI / 180.f); // to radians

    return hasChanged;
}
