#pragma once

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

class ComponentGenerator : public kodgen::MacroPropertyCodeGen
{
protected:
    inline static std::string propertyName = "ComponentGen";

    //std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property,
    //                                    rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

	virtual bool generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity, kodgen::Property const& property,
                                                  kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
                                                  std::string& inout_result) noexcept override;

public:
    ComponentGenerator() noexcept;
};