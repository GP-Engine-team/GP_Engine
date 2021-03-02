#include "SerializePropertyRule.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

SerializePropertyRule::SerializePropertyRule() noexcept :
	rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct | kodgen::EEntityType::Variable | kodgen::EEntityType::Field)
{

}

std::string SerializePropertyRule::generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property, rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
	// If entity is a class or a struct :
	if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) != kodgen::EEntityType::Undefined)
	{
		// Get Struct / Class info to access its fields
		kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);
		
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
				serializeInside += "serializer.serialize(" + ("node, s." + field.name) 
					+ "FieldInfo{" + field.name + ", " + field.type.getCanonicalName()+ "}"
					+ ");";
			}
		}

		std::string serializeFunction = "\
			template<typename SELF, typename SERIALIZER, typename NODE>				\
			static void serialize(SELF & s, SERIALIZER & serializer, NODE * node)	\
			{"
			+ serializeInside +
			"}";

		return "public:" + serializeFunction;
	}
	return "";
}