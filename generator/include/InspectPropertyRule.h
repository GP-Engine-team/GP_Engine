#pragma once

#include <RefurekuGenerator/Properties/DefaultComplexPropertyRule.h>

namespace kodgen
{
class StructClassInfo;
class ComplexProperty;
} // namespace kodgen

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


    std::string generateInspectFunction(const kodgen::StructClassInfo& entity,
                                       kodgen::ComplexProperty const& property, const std::string& functionName,
                                       const std::string& argClassName, const std::string& fieldCallingFunction,
                                       std::string extraQualifier = "") const;

public:
    InspectPropertyRule() noexcept;
};