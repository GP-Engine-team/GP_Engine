#pragma once

#include "Engine/Intermediate/DataChunk.hpp"
#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Refureku/TypeInfo/Properties/PropertySettings.h>

#include "Generated/ComponentGen.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class)) ComponentGen
    : public rfk::Property
{
    ComponentGen() noexcept = default;

    ComponentGen_GENERATED
};

File_GENERATED