#pragma once

#include <RefurekuGenerator/Properties/DefaultComplexPropertyRule.h>

class InspectPropertyRule : public rfk::DefaultComplexPropertyRule
{
protected:
    inline static std::string propertyName = "Inspect";

    // Code is injected in the [ClassName]_GENERATED macro
    std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
                                        rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

    // Code is injected in the File_GENERATED macro
    std::string generateFileFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
                                       rfk::PropertyCodeGenFileFooterData& data) const noexcept override;

public:
    InspectPropertyRule() noexcept;
};