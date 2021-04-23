#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"
#include "Engine/Intermediate/GameObject.hpp"

#include "Engine/ECS/Component/Component.hpp"

#include "Generated/Component.rfk.h"
File_GENERATED

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, Component& inspected)
{
    const char* name = inspected.getArchetype().name.c_str();

    if (!ImGui::CollapsingHeader(name))
        return;
    else
    {
        inspected.inspect(context);
    }
}

void GPE::Component::awake()
{

}