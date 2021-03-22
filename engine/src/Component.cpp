#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

#include "Engine/ECS/Component/Component.hpp"

File_GENERATED

bool GPE::Component::inspect()
{
    const char* name = getArchetype().name.c_str();
    //const char* name = getName();

    if (!ImGui::CollapsingHeader(name))
        return false;

    DataInspector::inspect(m_isActivated, *staticGetArchetype().getField("m_isActivated"));

    return true;
}