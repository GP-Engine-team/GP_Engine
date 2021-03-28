#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

#include "Engine/ECS/Component/Component.hpp"

template <>
void GPE::DataInspector::inspect(Component& inspected)
{
    const char* name = inspected.getArchetype().name.c_str();

    if (!ImGui::CollapsingHeader(name))
        return;
    else
    {
        inspected.inspect();
    }
}