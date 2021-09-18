#pragma once

#include <RefurekuGenerator/Parsing/FileParser.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenModule.h>

#include "ComponentGenerator.h"
#include "DefaultInspectPropertyRule.h"
#include "InspectPropertyRule.h"
#include "SerializePropertyRule.h"

//class CustomFileParserFactory 
class EngineCodeGenModule : public kodgen::MacroCodeGenModule
{
private:
    SerializePropertyRule      serializePropertyRule;
    InspectPropertyRule        inspectPropertyRule;
    ComponentGenerator         componentGenerator;
    DefaultInspectPropertyRule defaultInspectPropertyRule;

public:
    EngineCodeGenModule() noexcept
    {
        addPropertyCodeGen(serializePropertyRule);
        addPropertyCodeGen(inspectPropertyRule);
        addPropertyCodeGen(defaultInspectPropertyRule);
        addPropertyCodeGen(componentGenerator);
    }

    EngineCodeGenModule(const EngineCodeGenModule&) noexcept : EngineCodeGenModule()
    {

    }

    virtual EngineCodeGenModule* clone() const noexcept override
    {
        return new EngineCodeGenModule(*this);
    }


};