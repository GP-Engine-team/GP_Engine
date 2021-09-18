#pragma once

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

namespace kodgen
{
class StructClassInfo;
}

class SerializePropertyRule : public kodgen::MacroPropertyCodeGen
{
protected:
    inline static std::string propertyName = "Serialize";

    //std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
    //                                    rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

    //// Code is injected in the File_GENERATED macro
    //std::string generateFileFooterCode(kodgen::EntityInfo const& entity, kodgen::ComplexProperty const& property,
    //                                   rfk::PropertyCodeGenFileFooterData& data) const noexcept override;

	virtual bool generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity, kodgen::Property const& property,
                                                  kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
                                                  std::string& inout_result) noexcept override;

    virtual bool generateSourceFileHeaderCodeForEntity(kodgen::EntityInfo const& entity,
                                                    kodgen::Property const& property, kodgen::uint8
                                                    propertyIndex, kodgen::MacroCodeGenEnv& env,
                                                    std::string& inout_result) noexcept;

public:
    SerializePropertyRule() noexcept;
};