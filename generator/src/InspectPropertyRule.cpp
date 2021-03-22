#include "InspectPropertyRule.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

InspectPropertyRule::InspectPropertyRule() noexcept :
	rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct | kodgen::EEntityType::Variable | kodgen::EEntityType::Field)
{

}

std::string InspectPropertyRule::generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property, rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
	// If entity is a class or a struct :
	if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) != kodgen::EEntityType::Undefined)
	{
		// Get Struct / Class info to access its fields
		kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);
		
		std::string ifParentCalls = "";
		std::string serializeInside = "";

		// For each fields of the Reflected Class :
		for (auto& field : var.fields)
		{
			// Returns true if the property should be reflected (e.g. if it contains the Reflect Property), false otherwise.
			auto isPropertyReflected = [](const kodgen::ComplexProperty& property) { return property.mainProperty == propertyName; };

			// If the field should be reflected :
			auto& fieldProperties = field.properties.complexProperties;
			if (std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected) != fieldProperties.end())
			{
				std::string call1 = "c.getField(\""
					+ field.name
					+ "\")";

				serializeInside = "GPE::DataInspector::inspect(" + field.name + ", *" + call1 + ");";

				//serializeInside += "serializer.serialize(node, s." + field.name + ", "
				//	+ "FieldInfo{\"" + field.name + "\", \"" + field.type.getCanonicalName() + "\"}"
				//	+ ");";
			}

			std::cout << "REFUREKU GENERATOR IFPARENT : " << var.name << " >>>> nbParents : " << var.parents.size() << std::endl;
			ifParentCalls += "if (!(";
			for (auto& parent : var.parents)
			{
				std::cout << "REFUREKU GENERATOR ONPARENT" << std::endl;
				ifParentCalls += parent.type.getName() + "::inspect() && ";
			}
			ifParentCalls += "true))";
			ifParentCalls += "return false;";
		}

		std::string getArchetype = "rfk::Class const& c = " + entity.name + "::staticGetArchetype();";

		std::string serializeFunction = 
			"virtual bool inspect()"
			"{"
			+ ifParentCalls
			+ getArchetype
			+ serializeInside +
			"return true;"
			"}";

		std::cout << serializeFunction << std::endl;
		return "public:" + serializeFunction;
	}
	return "";
}