#pragma once

#include <RefurekuGenerator/Properties/DefaultComplexPropertyRule.h>

class InspectPropertyRule : public rfk::DefaultComplexPropertyRule
{
protected:
    inline static std::string propertyName = "Inspect";

    std::string generateClassFooterCode(kodgen::EntityInfo const& entity,
        kodgen::ComplexProperty const& property,
        rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

public:
    InspectPropertyRule() noexcept;
};