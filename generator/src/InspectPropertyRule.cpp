#include "InspectPropertyRule.h"
#include "GenerationUtilities.hpp"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

InspectPropertyRule::InspectPropertyRule() noexcept
    : rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct |
                                                        kodgen::EEntityType::Variable | kodgen::EEntityType::Field)
{
}

std::string InspectPropertyRule::generateClassFooterCode(kodgen::EntityInfo const&            entity,
                                                         kodgen::ComplexProperty const&       property,
                                                         rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
    // If entity is a class or a struct :
    if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
        kodgen::EEntityType::Undefined)
    {
        // Get Struct / Class info to access its fields
        kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

        return "public:" + generateSerializationFunction(var, property, "inspect", "GPE::InspectContext",
                                                             "GPE::DataInspector::inspect");
    }
    return "";
}

std::string InspectPropertyRule::generateFileFooterCode(kodgen::EntityInfo const&           entity,
                                                        kodgen::ComplexProperty const&      property,
                                                        rfk::PropertyCodeGenFileFooterData& data) const noexcept
{
    //// If entity is a class or a struct :
    //if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
    //    kodgen::EEntityType::Undefined)
    //{
    //    // Get Struct / Class info to access its fields
    //    kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

    //    return generateSerializationFunctionImpl(var, property, "inspect", "GPE::InspectContext",
    //                                             "GPE::DataInspector::inspect");
    //}
    return "";
}