#pragma once

#include <RefurekuGenerator/Properties/DefaultComplexPropertyRule.h>

class SerializePropertyRule : public rfk::DefaultComplexPropertyRule
{
protected:
    inline static std::string propertyName = "Serialize";

    std::string generateClassFooterCode(kodgen::EntityInfo const& entity,
        kodgen::ComplexProperty const& property,
        rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

public:
    SerializePropertyRule() noexcept;
};