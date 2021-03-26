#include "Engine/Serialization/GPMDataInspector.hpp"
#include "GPM/Transform.hpp"
#include "Engine/Serialization/Slider.hpp"
#include "imgui.h"

template <>
void GPE::DataInspector::inspect(GPM::SplitTransform& inspected, const rfk::Field& info)
{
    DataInspector::inspect(inspected.position, "Position");
    DataInspector::inspect(inspected.scale,    "Scale");
    //static inline bool DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max,
    //                              const char* format, float power)
    //{
    //    return DragScalarN(label, ImGuiDataType_Float, v, 3, v_speed, &v_min, &v_max, format, power);
    //}

    //static inline bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format,
    //                            float power)

    //ImGui::DragFloat3();
    //ImGui::SliderFloat3();

    //ImGui::DragVec3
    //ImGui::InputInt(info.name.c_str(), &inspected);
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
    ImGui::Text(name);
    ImGui::SameLine();
    ImGui::DragFloat3(name, inspected.e);
    // ImGui::InputFloat3(info.name.c_str(), &inspected, 0.1);
}