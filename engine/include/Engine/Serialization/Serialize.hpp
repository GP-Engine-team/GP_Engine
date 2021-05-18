#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Engine/Serialization/xml/xmlLoader.hpp> // alway used in serialization
#include <Engine/Serialization/xml/xmlSaver.hpp>  // alway used in serialization
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

#include "Generated/Serialize.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) Serialize : public rfk::Property
{
    Serialize() noexcept = default;
    Serialize(bool callParents) noexcept {};

    Serialize_GENERATED
};
