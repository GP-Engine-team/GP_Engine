#pragma once

#include <Refureku/TypeInfo/Properties/PropertySettings.h>

#include "Generated/Serialize.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) Serialize : public rfk::Property
{
    Serialize() noexcept = default;

    Serialize_GENERATED
};

File_GENERATED