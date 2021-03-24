#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

#include "Generated/ComponentGen.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class)) ComponentGen
    : public rfk::Property
{
    ComponentGen() noexcept = default;

    ComponentGen_GENERATED
};

File_GENERATED