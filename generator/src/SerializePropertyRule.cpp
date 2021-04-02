#include "SerializePropertyRule.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>
#include "GenerationUtilities.hpp"

SerializePropertyRule::SerializePropertyRule() noexcept
    : rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct |
                                                        kodgen::EEntityType::Variable | kodgen::EEntityType::Field)
{
}

//std::string SerializePropertyRule::generateSerializeFunction(const std::string&             argClassName,
//                                                             const kodgen::StructClassInfo& entity) noexcept
//{
//    std::string serializeInside = "";
//
//    std::string getArchetype = "rfk::Class const& c = " + entity.name + "::staticGetArchetype();";
//    serializeInside += getArchetype;
//
//    // For each fields of the Reflected Class :
//    for (auto& field : entity.fields)
//    {
//        // Returns true if the property should be reflected (e.g. if it contains the Reflect Property), false
//        auto isPropertyReflected = [](const kodgen::ComplexProperty& property) {
//            return property.mainProperty == propertyName;
//        };
//
//        // If the field should be reflected :
//        auto& fieldProperties = field.properties.complexProperties;
//        if (std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected) != fieldProperties.end())
//        {
//            std::string constructField = "c.getField(\"" + field.name + "\")";
//
//            serializeInside += entity.outerEntity->getFullName() +
//                               "::"
//                               "serialize(serializer, " +
//                               field.name + ", " + constructField + ");";
//        }
//    }
//
//    std::string serializeFunction = "\
//			virtual void serialize(" +
//                                    argClassName + "& serializer)\
//			{" + serializeInside + "}";
//
//    return serializeFunction;
//}

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
               generateSerializationFunction(var, property, "save", "XmlSaver", "GPE::save") +
               generateSerializationFunction(var, property, "load", "XmlLoader", "GPE::load");
    }
    return "";
}