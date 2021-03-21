#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"


bool GPE::Component::inspect()
{
    if (!ImGui::CollapsingHeader(getArchetype().name.c_str()))
        return false;

    DataInspector::inspect(m_isActivated, *staticGetArchetype().getField("m_isActivated"));

    return true;
}