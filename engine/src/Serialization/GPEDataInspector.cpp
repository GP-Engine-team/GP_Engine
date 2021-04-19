#include "Engine/Serialization/GPEDataInspector.hpp"

#include <imgui.h>

using namespace GPE;

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info)
{
    return GPE::DataInspector::inspect(context, inspected, info.name.c_str());
}

template <>
bool GPE::DataInspector::inspect(GPE::InspectContext& context, AmbiantComponent& inspected, const char* name)
{
    startProperty(name);
    const bool hasChanged = ImGui::ColorEdit4("", &inspected.e[0]);
    endProperty();

    return hasChanged;
}