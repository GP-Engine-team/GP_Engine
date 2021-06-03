#pragma once

#include "Engine/Serialization/xml/xmlManager.hpp"
#include <Refureku/TypeInfo/Properties/PropertySettings.h>
#include <string>

// Generated
#include "Generated/Separator.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Field)) Separator : public rfk::Property
{
    bool startSeparation = true;
    bool endSeparation   = true;

    Separator(bool startSeparation, bool endSeparation) : startSeparation(startSeparation), endSeparation(endSeparation)
    {
    
    }

    Separator_GENERATED
};
