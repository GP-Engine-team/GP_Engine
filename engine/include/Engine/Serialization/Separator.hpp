#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

// Generated
#include "Generated/Separator.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Field)) Separator : public rfk::Property
{
    bool startSeparation;
    bool endSeparation;

    Separator(bool startSeparation = true, bool endSeparation = false) : startSeparation(startSeparation), endSeparation(endSeparation)
    {
    
    }

    Separator_GENERATED
};
