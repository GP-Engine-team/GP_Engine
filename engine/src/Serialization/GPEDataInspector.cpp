#include "Engine/Serialization/GPEDataInspector.hpp"

#include <imgui.h>

using namespace GPE;

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGB& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit3("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGB& inspected)
{
    ImGui::ColorEdit3("", &inspected.e[0]);
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit4("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}

template <>
void GPE::DataInspector::inspect(InspectContext& context, RGBA& inspected)
{
    ImGui::ColorEdit4("", &inspected.e[0]);
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = inspect(context, inspected.rgba, name);
    endProperty();

    return hasChanged;
}