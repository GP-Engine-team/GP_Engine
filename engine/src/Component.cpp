#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

#include "Engine/ECS/Component/Component.hpp"

//bool GPE::Component::inspect()
//{
//    const char* name = getArchetype().name.c_str();
//
//    if (!ImGui::CollapsingHeader(name))
//        return false;
//
//    DataInspector::inspect(m_isActivated, *staticGetArchetype().getField("m_isActivated"));
//
//    return true;
//}

template <>
static void GPE::DataInspector::inspect(Component& inspected)
{
    const char* name = inspected.getArchetype().name.c_str();

    if (!ImGui::CollapsingHeader(name))
        return;
    else
    {
        inspected.inspect();
    }
}