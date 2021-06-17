#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Engine/Serialization/DataInspector.hpp>
#include <Engine/Serialization/InspectContext.hpp>
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

// Generated
#include "Generated/Inspect.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) Inspect : public rfk::Property
{
    Inspect() noexcept = default;

    // Used on fields
    Inspect(const char* setterFunction)
    {
    }

    // Used on classes / structs
    Inspect(bool callParents) noexcept
    {
    }

    Inspect_GENERATED
};
