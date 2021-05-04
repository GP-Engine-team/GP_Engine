#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

#include "Engine/ECS/Component/Component.hpp"

#include "Generated/Component.rfk.h"
File_GENERATED

    template <>
    void GPE::DataInspector::inspect(GPE::InspectContext& context, Component& inspected)
{
    inspected.inspect(context);
}