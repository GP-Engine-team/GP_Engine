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
		// What will be inside the generated "save" function.
		std::string saveFunctionInside;

		// What will be inside the generated "load" function.
		std::string loadFunctionInside;

		// Get Struct / Class info to access its fields
		kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

		auto saveLoadFunctionInsideGenerator = [&var](const std::string& instanceName) -> std::string
		{
			// What will be inside the generated function.
			std::string functionInside = "";

			// For each fields of the Reflected Class :
			for (auto& field : var.fields)
			{
				// Returns true if the property should be reflected (e.g. if it contains the Reflect Property), false otherwise.
				auto isPropertyReflected = [](const kodgen::ComplexProperty& property) { return property.mainProperty == propertyName; };

				// If the field should be reflected :
				auto& fieldProperties = field.properties.complexProperties;
				if (std::find_if(fieldProperties.begin(), fieldProperties.end(), isPropertyReflected) != fieldProperties.end())
				{
					functionInside += instanceName + '(' + "node," + field.name + ");";

					//functionInside += instanceName + '(' + "node," + field.name, ",\"" + field.name + "\","
					//	+ field.type.getCanonicalName(), ",\"" + field.type.getCanonicalName() + "\", \"\"" + ");";
				}
			}

			return functionInside;
		};
		
		std::string saveFunction = "\
			template<typename Saver, typename Node>			\
			void save(Saver& saver, Node* node) const		\
			{												"
			+ saveLoadFunctionInsideGenerator("saver") +
			"}";

		std::string loadFunction = "\
			template<typename Loader, typename Node>		\
			void load(Loader& loader, Node* node)			\
			{												"
			+ saveLoadFunctionInsideGenerator("loader") +
			"}";

		return "public:" + saveFunction + loadFunction ;
	}
	return "";
}