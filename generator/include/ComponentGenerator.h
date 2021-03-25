#pragma once

#include <RefurekuGenerator/Properties/DefaultSimplePropertyRule.h>

class ComponentGenerator : public rfk::DefaultSimplePropertyRule
{
protected:
    inline static std::string propertyName = "Serialize";

    std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property,
                                        rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

public:
    ComponentGenerator() noexcept;
};