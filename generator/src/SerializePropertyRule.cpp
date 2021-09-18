#include "SerializePropertyRule.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>
#include "GenerationUtilities.hpp"

SerializePropertyRule::SerializePropertyRule() noexcept
    : kodgen::MacroPropertyCodeGen(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct)
{
}

bool SerializePropertyRule::generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity,
    kodgen::Property const& property,
    kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
    std::string& inout_result) noexcept
{
    // If entity is a class or a struct :
    assert((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
           kodgen::EEntityType::Undefined);

    // Get Struct / Class info to access its fields
    kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

    // for (auto&& p : property.subProperties)
    //{
    //    generateSerializeFunction(p, var);
    //}
    inout_result += "public:" +
        generateSerializationFunctionDecl(var, property, "save", "XmlSaver", "GPE::save", "const") +
            generateSerializationFunctionDecl(var, property, "load", "XmlLoader", "GPE::load");

    return true;
}

bool SerializePropertyRule::generateSourceFileHeaderCodeForEntity(kodgen::EntityInfo const& entity,
    kodgen::Property const& property,
    kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
    std::string& inout_result) noexcept
{
    // If entity is a class or a struct :
    assert((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
           kodgen::EEntityType::Undefined);

    // Get Struct / Class info to access its fields
    kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

    inout_result += generateSerializationFunctionImpl(var, property, "save", "XmlSaver", "GPE::save", "const") +
            generateSerializationFunctionImpl(var, property, "load", "XmlLoader", "GPE::load");

    return true;
}