#pragma once

#include <string>
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include "Engine/Serialization/xml/xmlManager.hpp"
#include "Generated/Serialize.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) Serialize : public rfk::Property
{
    Serialize() noexcept = default;

    Serialize_GENERATED
};

File_GENERATED