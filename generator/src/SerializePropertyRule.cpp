#include "SerializePropertyRule.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>
#include "GenerationUtilities.hpp"

SerializePropertyRule::SerializePropertyRule() noexcept
    : rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct |
                                                        kodgen::EEntityType::Variable | kodgen::EEntityType::Field)
{
}

std::string SerializePropertyRule::generateClassFooterCode(kodgen::EntityInfo const&            entity,
                                                           kodgen::ComplexProperty const&       property,
                                                           rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
    // If entity is a class or a struct :
    if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
        kodgen::EEntityType::Undefined)
    {
        // Get Struct / Class info to access its fields
        kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

        //for (auto&& p : property.subProperties)
        //{
        //    generateSerializeFunction(p, var);
        //}
        return "public:" +
               generateSerializationFunctionDecl(var, property, "save", "XmlSaver", "GPE::save", "const") +
               generateSerializationFunctionDecl(var, property, "load", "XmlLoader", "GPE::load");
    }
    return "";
}

std::string SerializePropertyRule::generateFileFooterCode(kodgen::EntityInfo const&           entity,
                                                        kodgen::ComplexProperty const&      property,
                                                        rfk::PropertyCodeGenFileFooterData& data) const noexcept
{
    // If entity is a class or a struct :
    if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
        kodgen::EEntityType::Undefined)
    {
        // Get Struct / Class info to access its fields
        kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

        return generateSerializationFunctionImpl(var, property, "save", "XmlSaver", "GPE::save", "const") +
               generateSerializationFunctionImpl(var, property, "load", "XmlLoader", "GPE::load");
    }
    return "";
}