#include "Engine/Intermediate/GameObject.hpp"
#include "Engine/Serialization/DataInspector.hpp"
#include "imgui.h"

#include <Generated/Component.cpp.h>

GPE::Component::~Component() noexcept
{

}

template <>
void GPE::DataInspector::inspect(GPE::InspectContext& context, Component& inspected)
{
    context.lastComponentOwner = &inspected.getOwner();
    inspected.inspect(context);
}

void GPE::Component::onPostLoad()
{
    updateToSystem();
}