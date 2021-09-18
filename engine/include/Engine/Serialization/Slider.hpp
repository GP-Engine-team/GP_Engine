#pragma once

#include <Refureku/TypeInfo/Properties/PropertySettings.h>

#include <Engine/Serialization/xml/xmlManager.hpp>
#include <Generated/Slider.rfk.h>

struct RFKStruct(PropertySettings(rfk::EEntityKind::Struct | rfk::EEntityKind::Class | rfk::EEntityKind::Field |
                                  rfk::EEntityKind::Variable)) Slider : public rfk::Property
{
    float min = 0.0f;
    float max = 0.0f;

    Slider(float min, float max) noexcept : min(min), max(max)
    {
    }

    Slider_GENERATED
};

File_GENERATED