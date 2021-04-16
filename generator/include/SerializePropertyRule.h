#pragma once

#include <RefurekuGenerator/Properties/DefaultComplexPropertyRule.h>

namespace kodgen
{
class StructClassInfo;
}

class SerializePropertyRule : public rfk::DefaultComplexPropertyRule
{
protected:
    inline static std::string propertyName = "Serialize";

    std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
                                        rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

    // Code is injected in the File_GENERATED macro
    std::string generateFileFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
                                       rfk::PropertyCodeGenFileFooterData& data) const noexcept override;

public:
    SerializePropertyRule() noexcept;
};