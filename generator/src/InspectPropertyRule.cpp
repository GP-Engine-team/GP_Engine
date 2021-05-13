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

        return "public:" + generateInspectFunction(var, property, "inspect", "GPE::InspectContext",
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





std::string InspectPropertyRule::generateInspectFunction(const kodgen::StructClassInfo& entity,
                                                         kodgen::ComplexProperty const& property, const std::string& functionName,
                                                         const std::string& argClassName, const std::string& fieldCallingFunction,
                                                         std::string extraQualifier) const
{
    std::string serializeInside = "";

    if (property.subProperties.empty() || property.subProperties[0] == "true")
    {
        std::string callParents = "";
        for (auto& parent : entity.parents)
        {
            callParents += parent.type.getName() + "::" + functionName + "(serializer);";
        }

        serializeInside += callParents;
    }

    std::string getArchetype = "rfk::Struct const& c = " + entity.name + "::staticGetArchetype();";
    serializeInside += getArchetype;

    // For each fields of the Reflected Class :
    for (auto& field : entity.fields)
    {
        // Returns true if the property should be reflected (e.g. if it contains the correct Property), false otherwise
        auto isPropertyReflected = [&](const kodgen::ComplexProperty& prop) {
            return prop.mainProperty == property.mainProperty;
        };

        // If the field should be reflected :
        auto& fieldProperties = field.properties.complexProperties;
        auto  it              = std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected);
        if (it != fieldProperties.end())
        {
            if (it->subProperties.empty())
            {
                std::string constructField = "c.getField(\"" + field.name + "\")";

                serializeInside += fieldCallingFunction + "(serializer, " + field.name + ", *" + constructField + ");";
            }
            else
            {
                // If there is a setter
                std::string setterFunction = it->subProperties.front();
                setterFunction.erase(std::remove(setterFunction.begin(), setterFunction.end(), '"'), setterFunction.end());

                std::string setterCall = "{";
                setterCall += "auto temp = " + field.name + ";";
                setterCall += fieldCallingFunction + "(serializer, temp, \"" + field.name + "\");";
                setterCall += setterFunction + "(temp);";
                setterCall += "}";

                serializeInside += setterCall;
            }
        }
    }

    std::string serializeFunction = "\
			virtual void " + functionName +
                                    '(' + argClassName + "& serializer)" + extraQualifier + "{" + serializeInside + "}";

    std::cout << "Generated in " << entity.name << " : " << std::endl;
    std::cout << serializeFunction << std::endl;
    return serializeFunction;
}