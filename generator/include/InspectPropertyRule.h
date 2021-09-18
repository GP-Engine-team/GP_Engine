#pragma once

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

class InspectPropertyRule : public kodgen::MacroPropertyCodeGen
{
protected:
    inline static std::string propertyName = "Inspect";

    //// Code is injected in the [ClassName]_GENERATED macro
    //std::string generateClassFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property,
    //                                    rfk::PropertyCodeGenClassFooterData& data) const noexcept override;

    //// Code is injected in the File_GENERATED macro
    //std::string generateFileFooterCode(kodgen::EntityInfo const& entity, kodgen::Property const& property,
    //                                   rfk::PropertyCodeGenFileFooterData& data) const noexcept override;

    //std::string generateDefaultInspectFunction(const kodgen::StructClassInfo& entity,
    //                                           kodgen::Property const& property, const std::string& functionName,
    //                                           const std::string& argClassName, const std::string& fieldCallingFunction,
    //                                           std::string extraQualifier = "") const;

    virtual bool generateClassFooterCodeForEntity(kodgen::EntityInfo const& entity, kodgen::Property const& property,
                                                kodgen::uint8 propertyIndex, kodgen::MacroCodeGenEnv& env,
                                                std::string& inout_result) noexcept override;

    std::string generateInspectFunction(const kodgen::StructClassInfo& entity, kodgen::Property const& property,
                                        const std::string& functionName, const std::string& argClassName,
                                        const std::string& fieldCallingFunction, std::string extraQualifier = "") const;

    std::string generateDefaultInspectFunction(const kodgen::StructClassInfo& entity,
                                               kodgen::Property const& property, const std::string& functionName,
                                               const std::string& argClassName, const std::string& fieldCallingFunction,
                                               std::string extraQualifier = "") const;

public:
    InspectPropertyRule() noexcept;
};