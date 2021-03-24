#include "ComponentGenerator.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

ComponentGenerator::ComponentGenerator() noexcept :
	rfk::DefaultComplexPropertyRule(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct)
{

}

std::string ComponentGenerator::generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property, rfk::PropertyCodeGenClassFooterData& data) const noexcept
{
	// If entity is a class or a struct :
	if ((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) != kodgen::EEntityType::Undefined)
	{
		// Get Struct / Class info to access its fields
		kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

		std::string serializeInside = "DataChunk<" + var.name + ">::getInstance()->destroy(this);";

		std::string serializeFunction = "\
			void destroy() override 	 \
			{"
			+ serializeInside +
			"}";

		std::cout << serializeFunction << std::endl;
		return "public:" + serializeFunction;
	}
	return "";
}