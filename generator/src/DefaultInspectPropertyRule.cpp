#include "DefaultInspectPropertyRule.h"
#include "GenerationUtilities.hpp"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

DefaultInspectPropertyRule::DefaultInspectPropertyRule() noexcept
    : rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct)
{
}

std::string DefaultInspectPropertyRule::generateClassFooterCode(kodgen::EntityInfo const&            entity,
                                                         kodgen::ComplexProperty const&       property,
                                                         rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
    // If entity is a class or a struct :
    if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
        kodgen::EEntityType::Undefined)
    {
        // Get Struct / Class info to access its fields
        kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

        std::string defaultInspect = generateDefaultInspectFunction(var, property, "defaultInspect", "GPE::InspectContext",
                                                             "GPE::DataInspector::inspect");

        return "public :" + defaultInspect;
    }
    return "";
}

std::string DefaultInspectPropertyRule::generateFileFooterCode(kodgen::EntityInfo const&           entity,
                                                        kodgen::ComplexProperty const&      property,
                                                        rfk::PropertyCodeGenFileFooterData& data) const noexcept
{
    //// If entity is a class or a struct :
    // if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
    //    kodgen::EEntityType::Undefined)
    //{
    //    // Get Struct / Class info to access its fields
    //    kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

    //    return generateSerializationFunctionImpl(var, property, "inspect", "GPE::InspectContext",
    //                                             "GPE::DataInspector::inspect");
    //}
    return "";
}

std::string DefaultInspectPropertyRule::generateDefaultInspectFunction(
    const kodgen::StructClassInfo& entity, kodgen::ComplexProperty const& property, const std::string& functionName,
    const std::string& argClassName, const std::string& fieldCallingFunction, std::string extraQualifier) const
{
    

    std::string defaultInspectInside = "";

    std::string getArchetype         = "rfk::Struct const& c = " + entity.name + "::staticGetArchetype();";
    defaultInspectInside += getArchetype;

    // For each fields of the Reflected Class :
    for (auto& field : entity.fields)
    {
        // Returns true if the property should be reflected (e.g. if it contains the correct Property), false otherwise
        auto isPropertyReflected = [&](const kodgen::ComplexProperty& prop) {
            return prop.mainProperty == "Inspect";
        };

        // If the field should be reflected :
        auto& fieldProperties = field.properties.complexProperties;
        auto  it              = std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected);
        if (it != fieldProperties.end())
        {
            if (it->subProperties.empty())
            {
                std::string constructField = "c.getField(\"" + field.name + "\")";

                defaultInspectInside +=
                    fieldCallingFunction + "(serializer, " + field.name + ", *" + constructField + ");";
            }
            else
            {
                // If there is a setter
                std::string setterFunction = it->subProperties.front();
                setterFunction.erase(std::remove(setterFunction.begin(), setterFunction.end(), '"'),
                                     setterFunction.end());

                std::string setterCall = "{";
                setterCall += "auto temp = " + field.name + ";";
                setterCall += fieldCallingFunction + "(serializer, temp, \"" + field.name + "\");";
                setterCall += setterFunction + "(temp);";
                setterCall += "}";

                defaultInspectInside += setterCall;
            }
        }
    }

    std::string defaultInspect = "void " + functionName + '(' + argClassName + "& serializer)" + extraQualifier + '{' + defaultInspectInside + '}';

    return defaultInspect;
}