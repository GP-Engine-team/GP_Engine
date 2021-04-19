#pragma once

#include "DataInspector.hpp"
#include "Engine/Resources/Type.hpp"

namespace GPE
{
template <>
bool DataInspector::inspect(InspectContext& context, AmbiantComponent& inspected, const rfk::Field& info);

template <>
bool DataInspector::inspect(InspectContext& context, AmbiantComponent& inspected, const char* name);

} // namespace GPE