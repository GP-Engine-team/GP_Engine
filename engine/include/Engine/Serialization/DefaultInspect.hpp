#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

// Generated
#include "Generated/DefaultInspect.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) DefaultInspect : public rfk::Property
{
    DefaultInspect() noexcept = default;

    // Used on classes / structs
    DefaultInspect(bool callParents) noexcept {}; 

    DefaultInspect_GENERATED
};
