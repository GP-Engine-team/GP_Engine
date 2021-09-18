#include "ComponentGenerator.h"
#include <Kodgen/InfoStructures/StructClassInfo.h>
#include <algorithm>

ComponentGenerator::ComponentGenerator() noexcept
    : kodgen::MacroPropertyCodeGen(propertyName, kodgen::EEntityType::Class | kodgen::EEntityType::Struct)
{
}

bool ComponentGenerator::generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity,
                                                        kodgen::Property const& property,
                                            kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
                                            std::string& inout_result) noexcept 
{
    // If entity is a class or a struct :
    assert((entity.entityType & (kodgen::EEntityType::Class | kodgen::EEntityType::Struct)) !=
        kodgen::EEntityType::Undefined);

    // Get Struct / Class info to access its fields
    kodgen::StructClassInfo const& var = static_cast<kodgen::StructClassInfo const&>(entity);

    std::string newInside = "return GPE::DataChunk<" + var.name + ">::getInstance()->add();";

    std::string newFunction = "static void* operator new(std::size_t size)"
                                "{" +
                                newInside + "}";

    std::string deleteInside =
        "GPE::DataChunk<" + var.name + ">::getInstance()->destroy(static_cast<" + var.name + "*>(ptr));";

    std::string deleteFunction = "static void operator delete(void* ptr)"
                                    "{" +
                                    deleteInside + "}";

    std::cout << newFunction << deleteFunction << std::endl;
    inout_result += "public:" + newFunction + deleteFunction;
    return true;
}