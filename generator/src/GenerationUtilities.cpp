#include "GenerationUtilities.hpp"

#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

std::string generateSerializationFunction(const kodgen::StructClassInfo& entity,
                                          kodgen::ComplexProperty const& property, const std::string& functionName,
                                          const std::string& argClassName, const std::string& fieldCallingFunction,
                                          std::string extraQualifier)
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

    std::string getArchetype = "rfk::Class const& c = " + entity.name + "::staticGetArchetype();";
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
        if (std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected) != fieldProperties.end())
        {
            std::string constructField = "c.getField(\"" + field.name + "\")";

            serializeInside += fieldCallingFunction + "(serializer, " + field.name + ", *" + constructField + ");";
        }
    }

    std::string serializeFunction = "\
			virtual void " + functionName + '(' + argClassName + "& serializer)" + extraQualifier 
           + "{" + serializeInside + "}";

    std::cout << "Generated in " << entity.name << " : " << std::endl;
    std::cout << serializeFunction << std::endl;
    return serializeFunction;
}