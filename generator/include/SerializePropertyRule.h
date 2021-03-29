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

    //static std::string generateSerializeFunction(const std::string&             argClassName,
    //                                             const kodgen::StructClassInfo& entity) noexcept;

public:
    SerializePropertyRule() noexcept;
};